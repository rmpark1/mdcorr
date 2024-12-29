#include "fft.h"

namespace fft {

/*
 * fft implementation for forward and reverse. Interpret the input array
 * as a complex vector with half the size. Even components are real and
 * odd compenents are imaginary. Use 1/N inverse transform convention.
 *
 * @param input the input array (not const)
 * @param output the array to write the transform to.
 * @param rev the reverse fft flag, default is 0 (forward fft)
 */
template<class Iterator>
void complex_fft(Iterator arr, int size, int rev, int max_prime) {

    // Find the right set of primes for this size
    std::vector<int> primes = find_ideal_size(size, max_prime);
    int pmax = *std::max_element(primes.begin(), primes.end());
    double w_unit = -2*PI*rev;

    int right_size, nsub;
    int left_size = 1; // Size of the subproblem

    // Reorder array. Akin to bit reversal, but for a general prime decomposition.
    reorder(arr, primes);

    // Butterfly auxilary memory
    double store[2*pmax];

    // Loop through the layers of the algo tree, starting from the bottom.
    for (int l=0; l < primes.size(); l++) {

        right_size = left_size;
        left_size = right_size * primes[l];
        double Nfact = 1.0;
        nsub = size/left_size;
        for (int sub = 0; sub < size/left_size; sub++) { // Loop over left hand subproblems.
            for (int rk=0; rk < right_size; rk++) { // Loop over right hand k.

                // Compute butterfly via direct DFT
                for (int sl=0; sl < primes[l]; sl++) {
                    double Xk_real = 0; // FFT components for this subproblem
                    double Xk_im = 0; 
                    int lk = right_size*sl + rk;
                    for (int sr=0; sr < primes[l]; sr++) {
                        int r_id = sub*left_size + sr*right_size + rk;
                        double theta = w_unit*sr*lk/left_size;
                        double sth = sin(theta);
                        double cth = cos(theta);
                        Xk_real += cth*arr[2*r_id] - sth*arr[2*r_id+1];
                        Xk_im += sth*arr[2*r_id] + cth*arr[2*r_id+1];
                    }
                    store[2*sl] = Xk_real;
                    store[2*sl+1] = Xk_im;
                }
                // Copy back into arr
                for (int sl=0; sl < primes[l]; sl++) {
                    int l_id = left_size*sub + right_size*sl + rk;
                    arr[2*l_id] = store[2*sl];
                    arr[2*l_id+1] = store[2*sl+1];
                }
            }
        }
    }
    if (rev == -1) {
        for (int j = 0; j < size; j++) {
            arr[2*j] = arr[2*j] / size;
            arr[2*j+1] = arr[2*j+1] / size;
        }
    }
}

// Go operate forward on vector or in reverse.
template void complex_fft(std::vector<double>::iterator, int, int, int);
template void complex_fft(std::vector<double>::reverse_iterator, int, int, int);

/* Use complex fft with half the input values as real and half the input values
 * as complex.
 */
template<class Iterator>
void real_fft(Iterator input, int size, int rev, int max_prime) {
}
template void real_fft(std::vector<double>::iterator, int, int, int);
template void real_fft(std::vector<double>::reverse_iterator, int, int, int);

/* Increase the size until a size with prime factorization has no primes larger
 * than largetst prime. Returns the prime decomposition. Also, ensure there
 * is a factor of 2 in the decomposition so that the complex / real Fourrier
 * trick can be used.
 */
std::vector<int> find_ideal_size(int N, int largest_prime, int even) {
    
    if (N == 0) return std::vector<int>();
    if (N <= 3) return std::vector<int>{ N };

    bool found = false;
    int n = N;

    if (even) n = n + N%2;
    int max_prime;
    std::vector<int> primes;
    while (true) {
        // Find prime decomposition
        primes = get_prime_decomposion(n);
        max_prime = 0;
        // Find max and sum.
        std::for_each(primes.begin(), primes.end(),
            [&max_prime](int p) { max_prime = fmax(p, max_prime); });
        found = max_prime <= largest_prime;

        if (found) break;

        // Try next
        n++;
        if (even) n++;
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

// Reorder complex array
template<class Iterator>
void reorder(Iterator a, std::vector<int> primes) {
    // Compute prime
    int size = std::accumulate(primes.begin(), primes.end(), 1, std::multiplies<int>());
    int fid, rid;

    // Reorder mapping is a bit reversal in the prime encoding.
    PrimeEncoding encoding(primes);
    for (int k=0; k < size; k++) {
        fid = encoding.eval();
        encoding.reverse();
        rid = encoding.eval();
        // Only swap if greater
        if (rid > fid) {
            std::swap(a[2*rid], a[2*fid]);
            std::swap(a[2*rid+1], a[2*fid+1]);
        }
        encoding.reverse();
        encoding++;
    }
}
template void reorder(std::vector<double>::iterator, std::vector<int>);
template void reorder(std::vector<double>::reverse_iterator, std::vector<int>);

}
