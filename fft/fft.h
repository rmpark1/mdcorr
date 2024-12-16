#pragma once

#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>
#include <span>
#include <ranges>
#include <iostream>

#define PI 3.14159265358979323846

typedef std::span<double> span;

namespace fft {

template<class in_iter, class out_iter>
void base_fft(in_iter input, out_iter output, int size, int rev=1);

// void base_fft(span &input, span &output, int rev);
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

template<class T1, class T2, class T>
class Pair {

    T1 first;
    T2 second;

  public:

    Pair(T1 first, T2 second)
        : first(first), second(second) { };

    T &operator()(int i, int j) {
        if (i % 2 == 0) { return first[j]; }
        else { return second[j]; }
    }

    T operator()(int i, int j) const {
        if (i % 2 == 0) { return first[j]; }
        else { return second[j]; }
    }
};

template<class in_iter, class out_iter>
void reorder(in_iter input, out_iter output, std::vector<int> primes);

// Little endian prime encoding of an integer.
class PrimeEncoding {
    std::vector<int> &base;
    std::vector<int> rep;
  public:
    Encoding(std::vector<int> &base_) : base(base_), rep(base_.size()) { };
    
    void operator++() {
        int last = base.size()-1:
        rep[last] += 1;
        int remainder;
        for (int j=last; j>=0; j--) {
            int remainder = rep[j] % base[j];
        }
    }

    // 'Bit' flip
    void reverse() { std::reverse(rep.begin(), rep.end()); };

    int eval() {
        int sum = rep[last];
        for (int j = last-1; j>=0; j--) { sum += base[j+1]*rep[j]; }
        return sum;
    }
};

}
