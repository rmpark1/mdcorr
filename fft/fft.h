#pragma once

#include "array.h"

#include <algorithm>
#include <numeric>
#include <cmath>

typedef array::Arr3<double> A3;

namespace fft {

void forward(A3 &input, A3 &output);
void reverse(A3 &input, A3 &output);


/* Increase the size until a size with prime factorization has no primes larger
 * than largetst prime. Returns the prime decomposition
 */
std::vector<int> find_ideal_size(int N, int largest_prime=7);
std::vector<int> get_prime_decomposion(int n);
}


