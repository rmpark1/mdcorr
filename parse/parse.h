#pragma once

#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "array.h"

typedef std::string str;
typedef std::chrono::high_resolution_clock hr_clock;
typedef array::Arr3<double> A3;

#define verbose 1
#define NRUNS 2 // NVT, then NVE

namespace parse {

/**
 * Lammps Reader that will extract data by inference from the input file.
 * All data output must be relative to the input file.
 */
class LammpsReader {

  public:
    str directory;
    // Per file
    std::vector<str> paths;

    // Data dimensions
    int nspecies;
    int natoms;
    int nsteps;
    int nloaded;

    LammpsReader(str infile);
    ~LammpsReader();

    // Load all the data, careful for large files.
    int load(A3 &velocities);
    // Load subset of the data
    int load_range(array::Arr3<int> bounds);

    void read(double **);

    int check_dimensions();
};


std::vector<str> split(str s, str delimiter=str(" "));
std::vector<str> search_file(str fname, str match, int skip=0, str delimiter=str(" "));

}
