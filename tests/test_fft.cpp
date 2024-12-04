#include "fft.h"

#include <iostream>
#include <algorithm>
#include <vector>

bool is_decomposed_by(int N, std::vector<int> primes) {
    // Check if this is the correct prime factorization.
    double n = (double) N;
    for (int p : primes) {
        n = n / p;
    }
    return n == 1.0;
}

int test_find_size() {

    std::vector<int> sizes{0, 1, 20, 13, 100, 1000000};
    for (int size : sizes) {
        std::vector<int> primes = fft::find_ideal_size(size);
        int mult = std::accumulate(primes.begin(), primes.end(), 1, std::multiplies<int>());
        // std::cout << mult << std::endl;
        if (!is_decomposed_by(mult, primes)) return 1;
    }
    return 0;
}

int test_fft() {

    std::vector<double> a(10);
    return 0;
}

int test_s_access() {

    std::vector<double> a(10);
    std::iota(a.begin(), a.end(), 0);
    // std::reverse(a.begin(), a.end());

    std::ranges::reverse_view rv(a);

    for (auto v : rv) std::cout << v << std::endl;

    // for (auto v : a) std::cout << v << std::endl;

    std::span<double> s(a);
    std::span<double> s2(s.subspan(0, 3));
    // std::span<double> s3(rv.begin());

    s[1] = 39.;

    std::cout << "-----\n";
    for (auto s_ : s) std::cout << s_ << std::endl;

    std::cout << "-----\n";

    for (auto s_ : s2) std::cout << s_ << std::endl;


    return 0;
}

int main(void) {

    int status = 0;

    if (test_find_size()) status = 1;
    if (test_fft()) status = 2;
    if (test_s_access()) status = 3;

    std::cout << "END MAIN\nSTATUS: " << status << std::endl;
    return status;

}
