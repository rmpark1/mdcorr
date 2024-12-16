#include "fft.h"

#include <iostream>
#include <algorithm>
#include <vector>

namespace rv = std::ranges::views;

bool is_decomposed_by(int N, std::vector<int> primes) {
    // Check if this is the correct prime factorization.
    double n = (double) N;
    for (int p : primes) {
        n = n / p;
    }
    return n == 1.0;
}

int test_find_size() {

    std::vector<int> sizes{1, 20, 13, 100, 1000000};
    for (int size : sizes) {
        std::vector<int> primes = fft::find_ideal_size(size);
        int mult = std::accumulate(primes.begin(), primes.end(), 1, std::multiplies<int>());
        // std::cout << mult << std::endl;
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
    int N = 8; // complex size 2: 2

    std::vector<double> input(N);
    for (int j = 0; j < N/2; j++) {
        // input[j] = sin(.5*j) + cos(3.*j);
        input[2*j] = j;
        input[2*j+1] = 0;
    }
    // std::iota(input.begin(), input.end(), 0);
    std::vector<double> output(N);

    std::cout << "LAYER 0" << std::endl;
    for (int j = 0; j < N/2; j++) { std::cout << input[2*j] << " " << input[2*j+1] << std::endl; }
    std::cout << "\n\n"; 
    for (int j = 0; j < N/2; j++) { std::cout << output[2*j] << " " << output[2*j+1] << std::endl; }
    std::cout << "\n\n"; 

    fft::base_fft(input.begin(), output.begin(), N/2);

    std::cout << "END" << std::endl;
    for (int j = 0; j < N/2; j++) { std::cout << input[2*j] << " " << input[2*j+1] << std::endl; }
    std::cout << "\n\n"; 
    for (int j = 0; j < N/2; j++) { std::cout << output[2*j] << " " << output[2*j+1] << std::endl; }
    std::cout << "\n\n"; 


    return 0;
}

int main(void) {

    int status = 0;

    if (test_find_size()) status = 1;
    if (test_s_access()) status = 2;
    if (test_base_fft()) status = 3;

    std::cout << "END MAIN\nSTATUS: " << status << std::endl;
    return status;

}
