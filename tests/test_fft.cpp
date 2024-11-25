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
        std::cout << mult << std::endl;
        if (!is_decomposed_by(mult, primes)) return 1;
    }
    return 0;
}

int main(void) {

    int status = 0;

    if (test_find_size()) return 1;

    std::cout << status << std::endl;

}
