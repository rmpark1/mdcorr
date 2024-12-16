#include "fft.h"

namespace fft {

template<class in_iter, class out_iter>
void reorder(in_iter input, out_iter output, std::vector<int> primes) {
    // Compute prime
    std::vector<int> prime_acc(primes.size());
    std::inclusive_scan(
        primes.begin(), primes.end(), prime_acc.begin(), std::multiplies<int>());
    int size = prime_acc[primes.size()-1];

    // Reorder mapping is a bit reversal in the prime encoding.
    PrimeEncoding encoding(primes);
    printf("%d\n", 1/4);
    for (int k=0; k < size; k++) {
        // Get prime representation
        printf("SIZE: %d\n", prime_acc[k]);
    }
}
template void reorder(std::vector<double>::iterator, std::vector<double>::iterator, std::vector<int>);
template void reorder(std::vector<double>::reverse_iterator, std::vector<double>::iterator, std::vector<int>);

/*
 * fft implementation for forward and reverse. Interpret the input array
 * as a complex vector with half the size. Even components are real and
 * odd compenents are imaginary. Use 1/N inverse transform convention.
 *
 * @param input the input array (not const)
 * @param output the array to write the transform to.
 * @param rev the reverse fft flag, default is 0 (forward fft)
 */
template<class in_iter, class out_iter>
void base_fft(in_iter input, out_iter output, int size, int rev) {

    // Find the right set of primes for this size
    std::vector<int> primes = find_ideal_size(size);
    // Workspace
    Pair<in_iter, out_iter, double> ws(input, output);

    double w_unit = 2*PI*rev;

    int right_size, nsub;
    int left_size = 1; // Size of the subproblem

    // Reorder array. Akin to bit reversal, but for a general prime decomposition.
    reorder(input, output, primes);

    // Loop through the layers of the algo tree, starting from the bottom.
    for (int l=0; l < primes.size(); l++) {

        right_size = left_size;
        left_size = right_size * primes[l];
        double Nfact = fmin(1, pow(left_size, rev));
        nsub = size/left_size;
        for (int sub = 0; sub < nsub; sub++) { // Loop over left hand subproblems.
            for (int k=0; k < left_size; k++) { // Second loop over k for new size.

                double Xk_real = 0; // FFT components for this subproblem
                double Xk_im = 0; 

                int l_id = sub*left_size + k;

                // Compute butterfly
                for (int s=0; s < primes[l]; s++) {
                    int r_id = sub*left_size + s*right_size + (k % right_size);
                    double theta = w_unit*s*k/left_size;
                    double sth = sin(theta);
                    double cth = cos(theta);
                    Xk_real += cth*ws(l, 2*r_id) - sth*ws(l,2*r_id+1);
                    Xk_im += sth*ws(l, 2*r_id) + cth*ws(l,2*r_id+1);
                }
                ws(l+1, 2*l_id) = Xk_real*Nfact;
                ws(l+1, 2*l_id+1) = Xk_im*Nfact;
            }
        }
        std::cout << "LAYER " << l << std::endl;
        for (int j = 0; j < size; j++) { std::cout << input[2*j] << " " << input[2*j+1] << std::endl; }
        std::cout << "\n\n"; 
        for (int j = 0; j < size; j++) { std::cout << output[2*j] << " " << output[2*j+1] << std::endl; }
        std::cout << "\n\n"; 
    }
    if (primes.size() % 2 == 0) { // Copy back to output
        for(int k=0; k < 2*size; k++) output[k] = input[k];
    }
}
// Go operate forward on vector or in reverse.
template void base_fft(std::vector<double>::iterator, std::vector<double>::iterator, int, int);
template void base_fft(std::vector<double>::reverse_iterator, std::vector<double>::iterator, int, int);

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
    
    if (N == 0) return std::vector<int>();
    if (N <= 2) return std::vector<int>{ N };

    bool found = false;
    int n = N + (N % 2); // Only try even numbers
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
        n = n + 2;
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
