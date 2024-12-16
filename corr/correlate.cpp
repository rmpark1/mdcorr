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

/* N log(N) implementation of correlation */
void correlate_fft(A3 &in1, A3 &in2, A3 &output, int j, int k) {

    // Find the best padding for FFT
    std::vector<int> primes = fft::find_ideal_size(in1.h); // assume same for in2.
    int new_size = std::accumulate(primes.begin(), primes.end(), 1.0, std::multiplies<int>());

    // Add padding to prevent circular convolution
    new_size = new_size * 2;

    // Resize data
    in1.resize_contiguous(new_size, in1.w, in1.h);
    if (std::addressof(in1) != std::addressof(in2)) in2.resize_contiguous(new_size, in2.w, in2.d);
    output.resize_contiguous(new_size, output.w, output.h);

    // Form span over data

    // Run ffts
    // fft::real_forward()

}

void autocorrelate_fft(A3 &input, A3 &output) {
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

void autocorrelate(A3 &input, A3 &output, bool fft) { crosscorrelate(input, input, output, fft); }

void reduce(A3 &input, A3 &output) {

    double sum; 

    for (int i = 0; i < input.h; i++) {
        sum = 0;
        for (int j=0; j < input.w; j++) {
            for (int k=0; k < input.d; k++) {
                sum += input(i,j,k);
            }
        }
        output(i,0,0) = sum / (input.w * input.d);
    }
}

}
