#pragma once

#include <execution>
#include <numeric>
#include <functional>
#include <memory>

#include "array.h"


typedef array::Arr3<double> A3;

namespace corr {

/* Direct correlation implementation */
void correlate_direct(A3 &in1, A3 &in2, A3 &output, int j, int k);
/* FFT  correlation implementation. */
void correlate_fft(A3 &in1, A3 &in2, A3 &output, int j, int k);

void crosscorrelate(A3 &in1, A3 &in2, A3 &output, bool fft=false);

void autocorrelate(A3 &input, A3 &output, bool fft);

/* Average over particle and directional dimensions. */
void reduce(A3 &input, A3 &output);

}

