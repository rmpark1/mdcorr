#include <cmath>
#include <chrono>

#include "parse.h"
#include "array.h"
#include "correlate.h"

#include "c_api.cpp"

namespace chrono = std::chrono;
typedef chrono::steady_clock timer;

int main(int argc, char *argv[]) {

    // Parse user input
    parse::CLIReader cli(argc, argv);
    if (cli.help) return 0;

    // Parse LAMMPS input
    parse::LammpsReader data(cli.args);

    // Load all data into contiguous array
    int nsteps = fmax(data.nsteps, cli.args.timesteps) / cli.args.stride;
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
    A3 correlations(nsteps, data.natoms, 3);

    if (cli.args.verbose) std::cout << "Computing autocorrelation ... " << std::flush;

    const auto start = timer::now();

    corr::autocorrelate(velocities, correlations, cli.fft);

    const auto finish = timer::now();
    if (cli.args.verbose) {
        std::cout << "finished in "
        << chrono::duration_cast<chrono::seconds>(finish - start) << std::endl;
    }

    // Average
    A3 Z_sum(nsteps, 1, 1);
    corr::reduce(correlations, Z_sum);

    // Write file
    data.write_array(Z_sum);

    return 0;
}
