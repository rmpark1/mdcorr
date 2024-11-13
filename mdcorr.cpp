#include <cmath>

#include "parse.h"
#include "array.h"
#include "correlate.h"

#include "c_api.cpp"

int main(int argc, char *argv[]) {

    // Parse user input
    parse::CLIReader cli(argc, argv);
    parse::LammpsReader data(cli.input, cli.directory, cli.skip, cli.stride, cli.verbose);

    int nsteps = data.nsteps / cli.stride;
    // Load all data.
    A3 velocities(nsteps, data.natoms, 3);
    data.load(velocities);

    // Perform correlations
    std::cout << "Running autocorrelation\n";
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
