#include "correlate.h"
#include "fft.h"


namespace corr {

/* N^2 implemetation of correlation */
void correlate_direct(A3 &in1, A3 &in2, A3 &output, int j, int k) {
    double sum;
    for (int t=0; t < in1.h; t++) {
        sum = 0.0;
        for (int tp=0; tp < in1.h-t; tp++) {
            sum += in1(tp,j,k) * in2(t+tp,j,k);
        }
        output(t,j,k) = sum / (in1.h-t);
    }
}

/* Nlog(N) implementation of correlation */
void correlate_fft(A3 &in1, A3 &in2, A3 &output, int j, int k) {
    // NYI
}

void crosscorrelate(A3 &in1, A3 &in2, A3 &output, bool fft) {

    for (int k=0; k < in1.d; k++) {

        std::vector<int> atom_loop(in1.w);
        std::iota(atom_loop.begin(), atom_loop.end(), 0);

        // Create correlation mapping
        std::function<void(int)> correlate_1D;
        if (fft) { correlate_1D = [&k, &in1, &in2, &output](int j) {
            correlate_fft(in1, in2, output, j, k); };
        } else { correlate_1D = [&k, &in1, &in2, &output](int j) {
            correlate_direct(in1, in2, output, j, k); };
        }

        std::for_each(
            #ifdef PARALLEL
            std::execution::par_unseq,
            #else
            std::execution::seq,
            #endif
            atom_loop.begin(), atom_loop.end(), correlate_1D);
    }

}

void autocorrelate_1D(d_it x, int size) {

    fft::complex_fft(x, size, 1);

    for (int j = 0; j < size; j++) {
        double a = x[2*j];
        double b = x[2*j+1];
        x[2*j] = a*a + b*b;
        x[2*j+1] = 0.;
    }

    fft::complex_fft(x, size, -1);
};

void autocorrelate(A3 &data, int resize) {

    // Double array once for 0 padding and once for complex representation (for now)
    int size = data.h; // Size without padding
    if (!resize) size = size/4; // with padding

    // Find the best padding for FFT
    std::vector<int> primes = fft::find_ideal_size(2*size, 2, 1); // For now, only base 2
    int fft_size = std::accumulate(primes.begin(), primes.end(), 1.0, std::multiplies<int>());

    if (resize) data.resize_contiguous(2*fft_size, data.w, data.d);

    // Move 0, 1, 2, 3, ... to 0, 2, 4, 6 for complex representation.
    // Leave right hand side filled with 0s.
    to_complex_fmt(data, fft_size/2);

    // Pass in 1D time series arrays for each atom, spatial dimnesion.
    for (int k=0; k < data.d; k++) {

        std::vector<int> atom_loop(data.w);
        std::iota(atom_loop.begin(), atom_loop.end(), 0);

        // Create correlation mapping
        std::function<void(int)> correlate_1D = [&k, &data, &fft_size](int j) {
            autocorrelate_1D(data.begin_(j, k), fft_size);
        };

        std::for_each(
            #ifdef PARALLEL
            std::execution::par_unseq,
            #else
            std::execution::seq,
            #endif
            atom_loop.begin(), atom_loop.end(), correlate_1D);
    }
        

    // Undo move,
    to_real_fmt(data, size);

    if (resize) data.resize_contiguous(size, data.w, data.d);

}

void average(A3 &input, A3 &output) {
    double sum; 

    for (int i = 0; i < output.h; i++) {
        sum = 0;
        for (int j=0; j < input.w; j++) {
            for (int k=0; k < input.d; k++) {
                sum += input(i,j,k);
            }
        }
        output(i,0,0) = sum / (input.w * input.d * (output.h-i));
    }
}

void reduce(A3 &input, A3 &output) {
    double sum; 

    for (int i = 0; i < output.h; i++) {
        sum = 0;
        for (int j=0; j < input.w; j++) {
            for (int k=0; k < input.d; k++) {
                sum += input(i,j,k);
            }
        }
        output(i,0,0) += sum;
    }
}

void to_complex_fmt(A3 &data, int size) {
    for (int k = 0; k < data.d; k++) {
        for (int j = 0; j < data.w; j++) {
            for (int i = size; i >= 1; i--) {
                data(2*i-2, j, k) = data(i-1, j, k);
                data(2*i-1, j, k) = 0.;
            }
        }
    }
}

void to_real_fmt(A3 &data, int size) {
    for (int k = 0; k < data.d; k++) {
        for (int j = 0; j < data.w; j++) {
            for (int i = 0; i < size; i++) {
                data(i, j, k) = data(2*i, j, k);
            }
        }
    }
}


}
