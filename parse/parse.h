#pragma once

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <regex>
#include <limits>

#include "array.h"

namespace chrono = std::chrono;
typedef chrono::steady_clock timer;

typedef array::Arr3<double> A3;
typedef std::string str;

typedef std::size_t size_t;

#define IINFINITY std::numeric_limits<size_t>::max()

namespace parse {

struct LammpsSettings {
    str directory;
    str input;
    size_t skip;
    size_t stride;
    size_t timesteps;
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
    size_t skip;
    size_t stride; // Stride for this reader
    size_t dump_stride; // Stride for lammps output 
    size_t timesteps;

    str dump_path;
    size_t avg_size;
    std::vector<uns> col_map;

    // Data dimensions
    size_t nspecies;
    size_t natoms;
    size_t nsteps;

    LammpsReader(LammpsSettings args);

    void parse_log(LammpsSettings args);
    void print_summary();
    void build_col_map(std::vector<str> line);
    void check_dump();

    // Load all the data, careful for large files.
    size_t load(A3 &velocities, size_t atoms);
    // Load subset of the data
    size_t load_range(A3 &velocities, size_t min_atom, size_t max_atom);

    void load_step(A3 &velocities, size_t step, size_t min_atom=0, size_t max_atom=IINFINITY);
    size_t find_step(size_t step);

    size_t check_steps();

    void write_array(A3 &arr, str fname=str("correlations.dat"));

    void read(double **);
};


class CLIReader {
    
  public:
    LammpsSettings args;
    bool help;
    double mem;
    bool fft;
    bool direct;
    size_t max_atoms;
    str output;

    CLIReader(int argc, char *argv[]);
    void read_args(int argc, char *argv[]);
    void check_input();
};

std::vector<str> split(str s, str delimiter=str(" "));
std::vector<std::vector<str> > search_file(str fname, str match);
str get_parent(const str fname);

}
