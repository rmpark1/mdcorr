#include "correlate.h"

namespace corr {

void crosscorrelate(A3 &in1, A3 &in2, A3 &output) {

    for (int k=0; k < in1.d; k++) {

        std::vector<int> atom_loop(in1.w);
        std::iota(atom_loop.begin(), atom_loop.end(), 0);

        // Create correlation mapping
        auto single_atom_corr = [&k, &in1, &in2, &output](int j) {
            double sum;
            for (int t=0; t < in1.h; t++) {
                sum = 0.0;
                for (int tp=0; tp < in1.h-t; tp++) {
                    sum += in1(tp,j,k) * in2(t+tp,j,k);
                }
                output(t,j,k) = sum / (in1.h-t);
            }
        };

        std::for_each(
            #ifdef PARALLEL
            std::execution::par_unseq,
            #else
            std::execution::seq,
            #endif
            atom_loop.begin(),
            atom_loop.end(),
            single_atom_corr);
    }

}

void autocorrelate(A3 &input, A3 &output) { crosscorrelate(input, input, output); }

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
