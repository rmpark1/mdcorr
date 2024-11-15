#include <cmath>

#include "parse.h"
#include "array.h"
#include "correlate.h"

#include "c_api.cpp"

int main(int argc, char *argv[]) {

    // Parse user input
    parse::CLIReader cli(argc, argv);
    parse::LammpsReader data(cli.args);

    int nsteps = data.nsteps / cli.args.stride;
    // Load all data into contiguous array
    A3 velocities(nsteps, data.natoms, 3);
    int found = data.load(velocities);

    // If unexpected number of steps found, then copy into new array
    if (found != nsteps) velocities.resize_contiguous(found, data.natoms, 3);

    // Perform correlations
    if (cli.args.verbose) std::cout << "Running autocorrelation\n";
    fflush(stdout);
    A3 correlations(nsteps, data.natoms, 3);
    corr::autocorrelate(velocities, correlations);

    // Average
    A3 Z_sum(nsteps, 1, 1);
    corr::reduce(correlations, Z_sum);

    // Write file
    data.write_array(Z_sum);

    return 0;
}
