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
typedef array::Arr3<double> A3;

#define NRUNS 2 // NVT, then NVE

namespace parse {

struct Input {
    str directory;
    str input;
    int skip;
    int stride;
    bool verbose;
};

/**
 * Lammps Reader that will extract data by inference from the input file.
 * All data output must be relative to the input file.
 */
class LammpsReader {

  public:
    str directory;
    bool verbose;
    int skip;
    int stride;

    // Per file
    std::vector<str> paths;

    // Data dimensions
    int nspecies;
    int natoms;
    int nsteps;
    int nloaded;

    LammpsReader(Input args);

    // Load all the data, careful for large files.
    int load(A3 &velocities);
    // Load subset of the data
    int load_range(array::Arr3<int> bounds);

    void write_array(A3 &arr, str fname=str("correlations.dat"));

    void read(double **);

    int check_dimensions();
};


class CLIReader {
    
  public:
    Input args;

    CLIReader(int argc, char *argv[]);
    void read_args(int argc, char *argv[]);
    void check_input();
};

std::vector<str> split(str s, str delimiter=str(" "));
std::vector<str> search_file(str fname, str match, int skip=0, str delimiter=str(" "));
str get_parent(const str fname);

}
