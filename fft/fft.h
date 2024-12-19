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

template<class Iterator>
void complex_fft(Iterator input, int size, int rev=1, int max_prime=7);

// void base_fft(span &input, span &output, int rev);
template<class Iterator>
void real_fft(Iterator input, int size, int rev=1, int max_prime=7);

/* 
 * Increase the size until a size with prime factorization has no primes larger
 * than largetst prime. Returns the prime decomposition
 */
std::vector<int> find_ideal_size(int N, int largest_prime=7, int even=0);
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

template<class Iterator>
void reorder(Iterator arr, std::vector<int> primes);

// Little endian prime encoding of an integer. Ignore p[0] for encoding.
class PrimeEncoding {
    std::vector<int> &base;
    std::vector<int> rep;
    int rev;
  public:
    PrimeEncoding(std::vector<int> &base_)
        : base(base_), rep(base_.size()), rev(0) { };

    int flip(int i) {
        // Store encoding
        int n = base.size();
        std::vector<int> enc(n);
        for (int j = n-1; j >= 0; j--) {
            int remainder = i / base[j];
        }
        return n;
    }
    
    void operator++(int) {
        rep[0]++;
        for (int j=0; j < base.size()-1; j++) {
            if (rep[j] != base[j+1]) return;
            rep[j] = rep[j] % base[j+1];
            rep[j+1]++;
        }
        int last = base.size()-1;
        rep[last] = rep[last] % base[last]; // Overflow
    }

    // 'Bit' flip
    void reverse() {
        std::reverse(rep.begin(), rep.end());
        rev++;
        rev = rev % 2;
    };

    int eval() {
        int sum = rep[0];
        int weight = 1;
        for (int j=1; j < base.size(); j++) {
            weight = weight*base[j-rev];
            sum += weight*rep[j];
        }
        return sum;
    }
};

}
