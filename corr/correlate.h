#pragma once

#include <execution>
#include <numeric>
#include <functional>
#include <memory>

#include "array.h"


typedef array::Arr3<double> A3;
typedef std::vector<double>::iterator d_it;
typedef std::vector<double>::reverse_iterator r_it;

namespace corr {

/* Direct correlation implementation */
void autocorrelate_direct(A3 &input);
void autocorrelate_1D(d_it x, size_t size);
void autocorrelate(A3 &input, bool resize=1);

/* Average over particle and directional dimensions. */
void average(A3 &input, A3 &output);
void reduce(A3 &input, A3 &output);
void to_complex_fmt(A3 &data, size_t size);
void to_real_fmt(A3 &data, size_t size);

}

