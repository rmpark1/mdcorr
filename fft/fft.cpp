#include "fft.h"

namespace fft {

/* run correlation over first axis */
void forward(A3 &input, A3 &output) {

}

void reverse(A3 &input, A3 &output) {

}


/* Increase the size until a size with prime factorization has no primes larger
 * than largetst prime. Returns the prime decomposition
 */
std::vector<int> find_ideal_size(int N, int largest_prime) {
    
    bool found = false;
    int n = N;
    int max_prime;
    std::vector<int> primes;
    while (!found) {
        // Try next
        n++;
        // Find prime decomposition
        primes = get_prime_decomposion(n);
        max_prime = 0;
        // Find max and sum.
        std::for_each(primes.begin(), primes.end(),
            [&max_prime](int p) { max_prime = fmax(p, max_prime); });
        found = max_prime <= largest_prime;
    }
    return primes;
}

std::vector<int> get_prime_decomposion(int n) {

    std::vector<int> factors;

    if (n == 1) {
        factors.push_back(1);
        return factors;
    }

    while (n % 2 == 0) {
        factors.push_back(2);
        n = n / 2;
    }

    for (int i = 3; i <= sqrt(n); i=i+2) {
        while (n % i == 0) {
            factors.push_back(i);
            n = n/i;
        }
    }

    if (n > 2) factors.push_back(n);

    return factors;
}


}
