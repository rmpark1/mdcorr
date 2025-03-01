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
void complex_fft(Iterator arr, size_t size, int rev, uns max_prime) {

    // Find the right set of primes for this size
    std::vector<uns> primes = find_ideal_size(size, max_prime);
    uns pmax = *std::max_element(primes.begin(), primes.end());
    double w_unit = -2*PI*rev;

    size_t right_size;
    size_t left_size = 1; // Size of the subproblem

    // Reorder array. Akin to bit reversal, but for a general prime decomposition.
    reorder(arr, primes);

    // Butterfly auxilary memory
    double *store = new double[2*pmax];

    // Loop through the layers of the algo tree, starting from the bottom.
    for (size_t l=0; l < primes.size(); l++) {

        right_size = left_size;
        left_size = right_size * primes[l];
        for (size_t sub = 0; sub < size/left_size; sub++) { // Loop over left hand subproblems.
            for (size_t rk=0; rk < right_size; rk++) { // Loop over right hand k.

                // Compute butterfly via direct DFT
                for (size_t sl=0; sl < primes[l]; sl++) {
                    double Xk_real = 0; // FFT components for this subproblem
                    double Xk_im = 0; 
                    size_t lk = right_size*sl + rk;
                    for (size_t sr=0; sr < primes[l]; sr++) {
                        size_t r_id = sub*left_size + sr*right_size + rk;
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
                for (size_t sl=0; sl < primes[l]; sl++) {
                    size_t l_id = left_size*sub + right_size*sl + rk;
                    arr[2*l_id] = store[2*sl];
                    arr[2*l_id+1] = store[2*sl+1];
                }
            }
        }
    }
    if (rev == -1) {
        for (size_t j = 0; j < size; j++) {
            arr[2*j] = arr[2*j] / size;
            arr[2*j+1] = arr[2*j+1] / size;
        }
    }
    delete [] store;
}

// Go operate forward on vector or in reverse.
template void complex_fft(std::vector<double>::iterator, size_t, int, uns);
template void complex_fft(std::vector<double>::reverse_iterator, size_t, int, uns);

/* Use complex fft with half the input values as real and half the input values
 * as complex.
 */
// template<class Iterator>
// void real_fft(Iterator input, size_t size, int rev, uns max_prime) {
// }
// template void real_fft(std::vector<double>::iterator, size_t, int, uns);
// template void real_fft(std::vector<double>::reverse_iterator, size_t, int, uns);

/* Increase the size until a size with prime factorization has no primes larger
 * than largetst prime. Returns the prime decomposition. Also, ensure there
 * is a factor of 2 in the decomposition so that the complex / real Fourrier
 * trick can be used.
 */
std::vector<uns> find_ideal_size(size_t N, uns largest_prime) {
    
    if (N == 0) return std::vector<uns>();
    if (N <= 3) return std::vector<uns>{ static_cast<uns>(N) };

    size_t n = N;

    std::vector<uns> primes;
    while (true) {
        // Find prime decomposition
        primes = get_prime_decomposion(n, largest_prime);
        if (primes.size() > 0) break;
        // Try next
        n++;
    }
    return primes;
}

/* Get the prime decomposition up to some largest possible prime.
 * If it is not decomposable by that prime, return the empy set
 */
std::vector<uns> get_prime_decomposion(size_t n, uns largest_prime) {

    std::vector<uns> factors;

    if (n == 1) {
        factors.push_back(1);
        return factors;
    }

    while (n % 2 == 0) {
        factors.push_back(2);
        n = n / 2;
    }

    for (size_t i = 3; i <= largest_prime; i=i+2) {
        while (n % i == 0) {
            factors.push_back(i);
            n = n/i;
        }
    }

    if (n > 1) return std::vector<uns>();

    return factors;
}

// Reorder complex array
template<class Iterator>
void reorder(Iterator a, std::vector<uns> &primes) {
    // Compute prime
    size_t size = std::accumulate(
        primes.begin(), primes.end(), static_cast<size_t>(1), std::multiplies<size_t>());
    size_t fid, rid;

    // Reorder mapping is a bit reversal in the prime encoding.
    PrimeEncoding encoding(primes);
    for (size_t k=0; k < size; k++) {
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

template void reorder(std::vector<double>::iterator, std::vector<uns>&);
template void reorder(std::vector<double>::reverse_iterator, std::vector<uns>&);

}
