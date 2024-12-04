#include "fft.h"

namespace fft {

/* fft implementation for forward and reverse.
 * @param input the input array (not const)
 * @param output the array to write the transorm to.
 * @param rev the reverse fft flag, default is 0 (forward fft)
 */
void base_fft(span &input, span &output, int rev) {
}

/* Use complex fft with half the input values as real and half the input values
 * as complex.
 */
void base_real_fft(span &input, span &output, int rev) {
}


void forward(span &input, span &output) { return base_fft(input, output, 0); }
void reverse(span &input, span &output) { return base_fft(input, output, 1); }
void real_forward(span &input, span &output) { return base_real_fft(input, output, 0); }
void real_reverse(span &input, span &output) { return base_real_fft(input, output, 1); }


/* Increase the size until a size with prime factorization has no primes larger
 * than largetst prime. Returns the prime decomposition. Also, ensure there
 * is a factor of 2 in the decomposition so that the complex / real Fourrier
 * trick can be used.
 */
std::vector<int> find_ideal_size(int N, int largest_prime) {
    
    bool found = false;
    int n = N + (N % 2); // Only try even numbers
    int max_prime;
    std::vector<int> primes;
    while (!found) {
        // Try next
        n = n + 2;
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
