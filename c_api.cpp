#include <cmath>
#include <chrono>

#include "parse.h"
#include "array.h"
#include "correlate.h"
#include "fft.h"

namespace chrono = std::chrono;
typedef chrono::steady_clock timer;

extern "C" void autocorr(double *Z, int argc, char *argv[]) {

    // Parse user input
    parse::CLIReader cli(argc, argv);
    if (cli.help) return 0;

    // Parse LAMMPS input
    parse::LammpsReader data(cli.args);

    // Load all data into contiguous array
    int nsteps = fmin(data.nsteps, cli.args.timesteps) / cli.args.stride;
    A3 velocities(nsteps, data.natoms, 3);
    int found = data.load(velocities);

    // If unexpected number of steps found, then copy into new array
    if (found != nsteps) {
        if (cli.args.verbose) std::cout << "WARNING: Found less data than input specified "
            << found << "/" << nsteps << std::endl;
        nsteps = found;
        velocities.resize_contiguous(found, data.natoms, 3);
    }

    // Perform correlations
    // A3 correlations(nsteps, data.natoms, 3);

    if (cli.args.verbose) std::cout << "Computing autocorrelation ... " << std::flush;

    const auto start = timer::now();

    corr::autocorrelate(velocities); // velocities now hold correlations

    const auto finish = timer::now();
    if (cli.args.verbose) {
        std::cout << "finished in "
        << chrono::duration_cast<chrono::seconds>(finish - start) << std::endl;
    }

    // Average
    A3 Z_sum(nsteps, 1, 1);
    corr::reduce(velocities, Z_sum);

    // Convert to c array
    for (int i=0; i<nsteps; i++) Z[i] = Z_sum(i,0,0);
}

extern "C" void get_fft(double *Xr, double *Xi, double *xr, double *xi, int size) {

    std::cout << size << std::endl;
    std::vector<double> data(2*size);

    for (int j = 0; j < size; j++) {
        data[2*j] = xr[j];
        data[2*j+1] = xi[j];
    }

    fft::complex_fft(data.begin(), size);

    for (int j = 0; j < size; j++) {
        Xr[j] = data[2*j];
        Xi[j] = data[2*j+1];
    }
}
