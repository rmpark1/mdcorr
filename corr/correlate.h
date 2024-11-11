#pragma once

#include "array.h"

typedef array::Arr3<double> A3;

namespace corr {

/* Direct correlation implementation */
void crosscorrelate(A3 &in1, A3 &in2, A3 &output);
void autocorrelate(A3 &input, A3 &output);

}

