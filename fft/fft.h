#pragma once

#include "array.h"

typedef array::Arr3<double> A3;

namespace fft {

void forward(A3 &input, A3 &output);
void reverse(A3 &input, A3 &output);

}
