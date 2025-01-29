#include "fft.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>

namespace rv = std::ranges::views;

bool is_decomposed_by(size_t N, std::vector<uns> primes) {
    // Check if this is the correct prime factorization.
    double n = (double) N;
    std::cout << n << std::endl;
    for (uns p : primes) {
        n = n / p;
    }
    std::cout << n << std::endl;
    return n == 1.0;
}


int test_find_size() {

    std::vector<size_t> sizes{1, 20, 13, 100, 10000000000};
    for (size_t size : sizes) {
        std::vector<uns> primes = fft::find_ideal_size(size);

        size_t mult = std::accumulate(
            primes.begin(), primes.end(), static_cast<size_t>(1), std::multiplies<size_t>());
        if (!is_decomposed_by(mult, primes)) return 1;
    }
    return 0;
}

template<class Iterator>
int print_vector(Iterator it, std::size_t N) {
    for (int j=0; j < N; ++j) {
        std::cout << it[j] << std::endl;
    }
    return 0;
}

int test_s_access() {

    std::vector<double> a(10);
    std::iota(a.begin(), a.end(), 0);
    // std::reverse(a.begin(), a.end());

    // print_vector(a.begin(), a.size());
    // print_vector(a.rbegin(), a.size());

    // std::span<double> s(a);
    // std::span<double> s2(s.subspan(0, 3));
    // std::span<double> s3(rv.begin());
    //
    //
    // s[1] = 39.;

    // std::cout << "-----\n";
    // for (auto s_ : s) std::cout << s_ << std::endl;
    //
    // std::cout << "-----\n";
    //
    // for (auto s_ : s2) std::cout << s_ << std::endl;


    return 0;
}

int test_base_fft() {

    // Start with a good size array
    int size = 6;

    std::vector<double> xr(size);
    std::vector<double> xi(size);
    std::vector<double> Xr(size);
    std::vector<double> Xi(size);
    std::vector<double> data(2*size);

    for (int j = 0; j < size; j++) {
        xr[j] = j; // (double) (j % 5 == 0); //  *cos(2*PI*20*j/N);
    }

    for (int j = 0; j < size; j++) {
        data[2*j] = xr[j]; // (double) (j % 5 == 0); //  *cos(2*PI*20*j/N);
        data[2*j+1] = xi[j];
    }

    fft::complex_fft(data.begin(), size);

    for (int j = 0; j < size; j++) {
        Xr[j] = data[2*j];
        Xi[j] = data[2*j+1];
    }

    return 0;
}

int test_get_prime_decomposition() {

    size_t N = 786433;
    std::vector<uns> v= fft::find_ideal_size(N, 2);

    for (size_t j = 0; j < v.size(); j++) {
        std::cout << v[j] << std::endl;
    }
    std::cout << v.size() << std::endl;

    return 0;
}

int main(void) {

    int status = 0;

    // if (test_find_size()) status = 1;
    // if (test_s_access()) status = 2;
    // if (test_base_fft()) status = 3;
    if (test_get_prime_decomposition()) status = 4;

    std::cout << "END MAIN\nSTATUS: " << status << std::endl;
    return status;

}
