#include <cmath>

#include "parse.h"
#include "array.h"
#include "correlate.h"

// For now, tests in main.
void test_lammps_reader() {
    parse::LammpsReader data("data/lammps.in");
}


void corr_lammps(str infile) {

    parse::LammpsReader data("data/lammps.in");

    // Load all data.
    A3 velocities(data.nsteps, data.natoms, 3);
    data.load(velocities);

    A3 correlations(data.nsteps, data.natoms, 3);
    corr::autocorrelate(velocities, correlations);
}


int main(int argc, char *argv[]) {

    // test_lammps_reader();
    corr_lammps("data/lammps.in");

    return 0;
}
