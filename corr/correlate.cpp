#include "correlate.h"

namespace corr {

/* run correlation over first axis */
void crosscorrelate(A3 &in1, A3 &in2, A3 &output) {

    double sum;

    for (int k=0; k < in1.d; k++) {
        for (int j=0; j < in1.w; j++) {

            // Perform correlation
            for (int t=0; t < in1.h; t++) {
                sum = 0.0;
                for (int tp=0; tp < in1.h-t; tp++) {
                    sum += in1(tp,j,k) * in2(t+tp,j,k);
                }
                output(t,j,k) = sum / (in1.h-t);
            }
        }
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
