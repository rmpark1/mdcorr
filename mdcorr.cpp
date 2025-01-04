#include <cmath>
#include <chrono>

#include "parse.h"
#include "array.h"
#include "fft.h"
#include "correlate.h"

namespace chrono = std::chrono;
typedef chrono::steady_clock timer;

void full_autocorr(parse::CLIReader &cli, parse::LammpsReader &data) {

    // Load all data into contiguous array
    size_t nsteps = data.check_steps();
    size_t natoms = std::min(cli.max_atoms, data.natoms);

    A3 velocities(nsteps, natoms, 3);

    data.load(velocities, natoms);

    printf("Found %zu time steps\n", nsteps);
    fflush(stdout);

    if (cli.args.verbose) std::cout << "Computing autocorrelation ... " << std::flush;

    const auto start = timer::now();

    if (cli.direct) {
        corr::autocorrelate_direct(velocities); // velocities now hold correlations
    } else {
        corr::autocorrelate(velocities, 1); // velocities now hold correlations
    }

    const auto finish = timer::now();
    if (cli.args.verbose) {
        std::cout << "finished in "
        << chrono::duration_cast<chrono::seconds>(finish - start) << std::endl;
    }

    // Average
    A3 Z_sum(nsteps, 1, 1);

    corr::average(velocities, Z_sum);

    // Write file
    data.write_array(Z_sum, cli.output);
}

void chunk_autocorr(parse::CLIReader cli, parse::LammpsReader &data) {

    // Get actual size from null readout
    size_t nsteps = data.check_steps();

    printf("Found %zu time steps\n", nsteps);
    fflush(stdout);

    size_t array_size = cli.mem*pow(2,20)/(4*sizeof(double));
    size_t natoms = std::min(cli.max_atoms, data.natoms);
    size_t chunk = std::min(natoms, array_size / (nsteps*3));
    size_t nchunks = std::ceil(static_cast<double>(natoms) / chunk);

    // One array for all chunks. Use ffts - need 4x space (for now)
    std::vector<uns> primes = fft::find_ideal_size(2*nsteps, 2); // For now, only base 2
    size_t fft_size = std::accumulate(
        primes.begin(), primes.end(), static_cast<size_t>(1), std::multiplies<size_t>());

    A3 velocities(2*fft_size, chunk, 3);
    A3 Z_sum(nsteps, 1, 1);

    if (cli.args.verbose) std::cout << "Start chunk loading for "
        << nchunks << " chunks" << std::endl;
    for (size_t n=0; n<nchunks; n++) {

        size_t chunk_size = chunk;
        if (n == nchunks-1) {
            chunk_size = natoms-n*chunk;
            // velocities.resize_contiguous(2*fft_size, chunk_size, 3);
        }

        const auto start = timer::now();

        data.load_range(velocities, n*chunk, n*chunk+chunk_size);

        corr::autocorrelate(velocities, 0); // velocities now hold correlations

        const auto finish = timer::now();

        switch (cli.args.verbose) {
            case 1:
                std::cout << n+1 << "/" << nchunks << " finished in "
                << chrono::duration_cast<chrono::seconds>(finish - start) << std::endl;
            default: {}
        }

        // Average
        corr::reduce(velocities, Z_sum);

        if (n < nchunks-1) {
            velocities.fill_range(0, velocities.h, 0.0);
        }
    }


    // Divide over natoms
    for (size_t i = 0; i < Z_sum.h; i++) {
        Z_sum[i] = Z_sum[i] / (natoms * 3 * (nsteps-i));
    }

    // Write file
    data.write_array(Z_sum, cli.output);
}

int main(int argc, char *argv[]) {

    // Parse user input
    parse::CLIReader cli(argc, argv);
    if (cli.help) return 0;

    // Parse LAMMPS input
    parse::LammpsReader data(cli.args);

    if (cli.mem == 0) full_autocorr(cli, data);
    else chunk_autocorr(cli, data);

    return 0;
}
