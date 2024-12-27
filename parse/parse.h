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

#define IINFINITY std::numeric_limits<int>::max()

namespace parse {

struct LammpsSettings {
    str directory;
    str input;
    int skip;
    int stride;
    unsigned int timesteps;
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
    int stride; // Stride for this reader
    int dump_stride; // Stride for lammps output 
    unsigned int timesteps;

    str dump_path;
    int avg_size;
    std::vector<int> col_map;

    // Data dimensions
    int nspecies;
    int natoms;
    int nsteps;
    int nloaded;

    LammpsReader(LammpsSettings args);

    void parse_log(LammpsSettings args);
    void print_summary();
    void build_col_map(std::vector<str> line);
    void check_dump();

    // Load all the data, careful for large files.
    int load(A3 &velocities);
    // Load subset of the data
    int load_range(A3 &velocities, int min_atom, int max_atom);

    void load_step(A3 &velocities, int step, int min_atom=0, int max_atom=IINFINITY);
    unsigned long find_step(int step);

    int check_steps();


    void write_array(A3 &arr, str fname=str("correlations.dat"));

    void read(double **);
};


class CLIReader {
    
  public:
    LammpsSettings args;
    int help;
    int mem;
    bool fft;
    unsigned int max_atoms;

    CLIReader(int argc, char *argv[]);
    void read_args(int argc, char *argv[]);
    void check_input();
};

std::vector<str> split(str s, str delimiter=str(" "));
std::vector<std::vector<str> > search_file(str fname, str match);
str get_parent(const str fname);

}
