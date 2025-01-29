#pragma once

#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>
#include <span>
#include <ranges>
#include <iostream>

#define PI 3.14159265358979323846

typedef std::size_t size_t;
typedef unsigned short uns;

namespace fft {

template<class Iterator>
void complex_fft(Iterator arr, size_t size, int rev=1, uns max_prime=7);

// void base_fft(span &input, span &output, int rev);
// template<class Iterator>
// void real_fft(Iterator input, size_t size, size_t rev=1, uns max_prime=7);

/* 
 * Increase the size until a size with prime factorization has no primes larger
 * than largetst prime. Returns the prime decomposition
 */
std::vector<uns> find_ideal_size(size_t N, uns largest_prime=7);
std::vector<uns> get_prime_decomposion(size_t n, uns largeset_prime);

template<class Iterator>
void reorder(Iterator arr, std::vector<uns> &primes);

// Little endian prime encoding of an integer. Ignore p[0] for reverse encoding,
// and p[-1] forward encoding.
class PrimeEncoding {
    std::vector<uns> &base;
    std::vector<uns> rep;
    size_t rev;
    std::vector<uns> A0; // Cumulative products
    std::vector<uns> A1;
  public:
    PrimeEncoding(std::vector<uns> &base_)
        : base(base_), rep(base_.size()), rev(0) { 

        // std::inclusive_scan(base.begin(), base.end()-1, A0, 1, std::multiplies<size_t>());
        // std::inclusive_scan(base.begin()+1, base.end(), A1, 1, std::multiplies<size_t>());

    };

    size_t flip(size_t k) {
        // Represent forward encoding
        // size_t n = base.size();

        // for (; k >= 0; k--) {
        // }

        // for (size_t j=0; j < base.size()-1; j++) {
        //     if (rep[j] != base[j+1]) return;
        //     rep[j] = rep[j] % base[j+1];
        //     rep[j+1]++;
        // }
        // size_t last = base.size()-1;
        // rep[last] = rep[last] % base[last]; // Overflow
        return k;
    }
    
    void operator++(int) {
        rep[0]++;
        for (size_t j=0; j < base.size()-1; j++) {
            if (rep[j] != base[j+1]) return;
            rep[j] = rep[j] % base[j+1];
            rep[j+1]++;
        }
        size_t last = base.size()-1;
        rep[last] = rep[last] % base[last]; // Overflow
    }

    // 'Bit' flip
    void reverse() {
        std::reverse(rep.begin(), rep.end());
        rev++;
        rev = rev % 2;
    };

    size_t eval() {
        size_t sum = rep[0];
        size_t weight = 1;
        for (uns j=1; j < base.size(); j++) {
            weight = weight*base[j-rev];
            sum += weight*rep[j];
        }
        return sum;
    }
};

}
