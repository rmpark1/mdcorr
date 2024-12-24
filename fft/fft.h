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

template<class Iterator>
void reorder(Iterator arr, std::vector<int> primes);

// Little endian prime encoding of an integer. Ignore p[0] for reverse encoding,
// and p[-1] forward encoding.
class PrimeEncoding {
    std::vector<int> &base;
    std::vector<int> rep;
    int rev;
    std::vector<int> A0; // Cumulative products
    std::vector<int> A1;
  public:
    PrimeEncoding(std::vector<int> &base_)
        : base(base_), rep(base_.size()), rev(0) { 

        // std::inclusive_scan(base.begin(), base.end()-1, A0, 1, std::multiplies<int>());
        // std::inclusive_scan(base.begin()+1, base.end(), A1, 1, std::multiplies<int>());

    };

    int flip(int k) {
        // Represent forward encoding
        int n = base.size();

        for (; k >= 0; k--) {
        }

        // for (int j=0; j < base.size()-1; j++) {
        //     if (rep[j] != base[j+1]) return;
        //     rep[j] = rep[j] % base[j+1];
        //     rep[j+1]++;
        // }
        // int last = base.size()-1;
        // rep[last] = rep[last] % base[last]; // Overflow
        return k;
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
