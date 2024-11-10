#pragma once

#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <string_view>


#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

#include "array.h"

typedef std::string str;

#define verbose 1

namespace parse {

class LammpsReader {
  private:

    // Per file
    std::vector<str> paths;
    std::stringstream data_stream;

    // Data dimensions
    int nsteps;
    std::vector<int> steps;

    // Data
    int *ids;
    int *time_steps;
    double *positions;
    double *velocities;

  public:
    LammpsReader(str directory, str lammps_out, str suffix=str(".dat"));
    LammpsReader(str directory, int nsteps, str suffix=str(".dat"));
    ~LammpsReader();

    void find_dump_files(str directory, str suffix);

    // Add a dump file manually
    int add_file(str fname);

    // Infer nsteps from main output.
    int get_nsteps(str fname);

    // Load all the data, careful for large files.
    int load();
    // Load subset of the data
    int load_range(int start, int stop);

    void read(double **);

    int check_dimensions();
};

}
