#include "parse.h"

str os_sep =
#ifdef _WIN32
"\\";
#else
"/";
#endif

namespace parse {

/**
 * Lammps Reader that will extract data by inferring from the input file.
 *
 * @param infile the LAMMPS input file.
 */
LammpsReader::LammpsReader(str infile) {

    // Infer directory relative to the infile
    std::vector<str> parts = split(infile, os_sep);
    for (int i=0; i < parts.size()-1; i++) directory += parts[i];

    // Infer nsteps from output file
    std::vector<str> tokens;

    tokens = search_file(infile, "run", NRUNS-1);
    int n_sim_steps = std::stoi(tokens[1]);

    tokens = search_file(infile, "dump");
    int stride = std::stoi(tokens[4]);
    str dump_path = tokens[5];
    paths.push_back(dump_path);

    // Read number of species and number of each species
    nspecies = 0;
    natoms = 0;
    while (true) {
        tokens = search_file(infile, "create_atoms", nspecies);
        if (tokens.size() == 0) break;
        nspecies++;
        natoms += std::stoi(tokens[3]);
    }

    nsteps = (n_sim_steps / stride) + 1;

    if (verbose) {
        printf("----------------\n");
        printf("Reading LAMMPS input\n");
        printf("----------------\n");
        printf("Calculation directory: %s\n", directory.c_str());
        printf("# of species: %i\n", nspecies);
        printf("# of atoms: %i\n", natoms);
        printf("# of simulation steps: %i\n", n_sim_steps);
        printf("# of dump steps: %i\n", nsteps);
        printf("Stride: %i\n", stride);
        printf("----------------\n\n");
    }
}

LammpsReader::~LammpsReader() {
}

/**
 * Read all data in a specified directory
 */
int LammpsReader::load(A3 &velocities) {

    double vx, vy, vz;
    int id;
    str line;
    int nsteps_found = 0;

    // Open all file streams
    for (auto &fname : paths) {

        std::fstream file_handle(directory+os_sep+fname);
        if(verbose) std::cout << "reading " << fname << "\n\n";
        std::getline(file_handle, line);

        while (!file_handle.eof()) {

            for (int i=0; i<8; i++) std::getline(file_handle, line);

            for (int i=0; i< natoms; i++) {
                file_handle >> id >> vx >> vy >> vz;
                velocities(nsteps_found, id-1, 0) = vx;
                velocities(nsteps_found, id-1, 1) = vy;
                velocities(nsteps_found, id-1, 2) = vz;
                // std::cout
                // << velocities(nsteps_found, id, 0) << " "
                // << velocities(nsteps_found, id, 1) << " "
                // << velocities(nsteps_found, id, 2) << "\n";
            }
            nsteps_found++;
            std::getline(file_handle, line);
            std::getline(file_handle, line);
        }

        if (nsteps_found != nsteps) {
            throw std::length_error("Not all time steps found");
        }
    }
    return 0;
}

int LammpsReader::load_range(array::Arr3<int> bounds) {
    return 0;
}

/* UTILITIES */

std::vector<str> search_file(str fname, str match, int skip, str delimiter) {

    std::fstream instream(fname);
    str line;
    std::getline(instream, line);

    for (int irun = 0; irun <= skip; irun++) {
        while (line.find(match) == str::npos) {
            std::getline(instream, line);
            if (instream.eof()) return std::vector<str>();
        }
        if (irun < skip) std::getline(instream, line);
    }
    std::vector<str> tokens = split(line, delimiter);
    return tokens;
}

std::vector<str> split(str s, str delimiter) {
    int start_pos = 0;
    int end_pos = 0;
    std::vector<str> tokens;
    while ((end_pos = s.substr(start_pos, s.length()).find(delimiter)) != str::npos) {
        tokens.push_back(s.substr(start_pos, end_pos));
        start_pos += end_pos + delimiter.length();
    }
    tokens.push_back(s.substr(start_pos, s.length()));

    return tokens;
}
    
}
