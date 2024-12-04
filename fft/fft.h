#pragma once

#include <algorithm>
#include <cmath>
#include <complex>
#include <numeric>
#include <vector>
#include <span>
#include <ranges>

typedef std::span<double> span;
// typedef std::ranges::view view;

namespace fft {

void base_fft(span &input, span &output, int rev);
void base_real_fft(span &input, span &output, int rev);

void forward(span &input, span &output);
void reverse(span &input, span &output);

void real_forward(span &input, span &output);
void real_reverse(span &input, span &output);

/* 
 * Increase the size until a size with prime factorization has no primes larger
 * than largetst prime. Returns the prime decomposition
 */
std::vector<int> find_ideal_size(int N, int largest_prime=7);
std::vector<int> get_prime_decomposion(int n);

}
