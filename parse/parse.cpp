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
LammpsReader::LammpsReader(
        str infile, str directory_, int skip, int stride, bool verbose) :
        verbose(verbose), skip(skip), stride(stride), directory(directory_) {

    if (directory.empty()) directory = get_parent(infile);

    // Infer nsteps from output file
    std::vector<str> tokens;

    tokens = search_file(infile, "run", NRUNS-1);
    int n_sim_steps = std::stoi(tokens[1]);

    tokens = search_file(infile, "dump");
    int dump_stride = std::stoi(tokens[4]);
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

    nsteps = (n_sim_steps / dump_stride) + 1;

    if (verbose) {
        printf("----------------\n");
        printf("Reading LAMMPS input\n");
        printf("----------------\n");
        printf("Calculation directory: %s\n", directory.c_str());
        printf("# of species: %i\n", nspecies);
        printf("# of atoms: %i\n", natoms);
        printf("# of simulation steps: %i\n", n_sim_steps);
        printf("# of dump steps: %i\n", nsteps);
        printf("LAMMPS dump stride: %i\n", dump_stride);
        printf("# of correlation points: %i\n", nsteps/stride);
        printf("----------------\n");
    }
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
        if(verbose) std::cout << "reading " << fname << "\n";
        std::getline(file_handle, line);

        while (!file_handle.eof()) {

            for (int i=0; i<8; i++) std::getline(file_handle, line);

            for (int i=0; i< natoms; i++) {
                file_handle >> id >> vx >> vy >> vz;
                if (i % stride == 0) {
                    velocities(nsteps_found, id-1, 0) = vx;
                    velocities(nsteps_found, id-1, 1) = vy;
                    velocities(nsteps_found, id-1, 2) = vz;
                }
            }
            nsteps_found++;

            std::getline(file_handle, line);
            std::getline(file_handle, line);

            // if (verbose) {
            //     printf("Found step %d/%d", nsteps_found, nsteps);
            //     if (!file_handle.eof()) { std::cout << "\r"; }
            //     else { std::cout << "\n"; }
            //     fflush(stdout);
            // }

            if (nsteps_found >= nsteps) break;
        }

        if (nsteps_found != nsteps) {
            throw std::length_error("Not all time steps found");
        }
    }
    return 0;
}

int LammpsReader::load_range(array::Arr3<int> bounds) {
    return 0; // NYI
}

void LammpsReader::write_array(A3 &arr, str fname) {

    std::fstream file(directory+os_sep+fname, std::fstream::out);

    for (int i=0; i<arr.h; i++) {
        for (int j=0; j<arr.w; j++) {
            for (int k=0; k<arr.d; k++) {
                file << arr(i,j,k);
            }
        }
        if (i < arr.h-1) file << "\n";
    }
}

CLIReader::CLIReader(int argc, char *argv[]) {

    // Default settings
    directory = "";
    input = "";
    skip = 1;
    stride = 1;
    verbose = 0;

    read_args(argc, argv);
    check_input();
}

void CLIReader::read_args(int argc, char *argv[]) {
    // Parse arguments
    int remaining = argc - 1;
    str arg, arg_val;

    while (remaining >= 1) {
        arg = str(argv[argc-remaining]);
        auto match = [&arg](str s1, str s2) {
            return ((str(arg) == s1) | (str(arg)==(s2))); };

        if (match("--verbose", "-v")) {
            verbose = 1;
            remaining -= 1;
            continue;
        }

        arg_val = str(argv[argc-remaining+1]);
        if (match("--input", "-i")) { input = str(arg_val); remaining -= 2; }
        if (match("--directory", "-d")) { directory = str(arg_val); remaining -= 2; }
        if (match("--skip", "-s")) { skip = std::stoi(arg_val); remaining -= 2; }
        if (match("--stride", "-j")) { stride = std::stoi(arg_val); remaining -= 2; }
    }
}

void CLIReader::check_input() {

    bool invalid = false;
    if (input.empty()) {
            printf("Missing input file argument: '--input <lammps_input>'\n");
            invalid = true;
    }

    if (invalid) {
        std::cout <<
            "Usage:\n"
            "mdcorr --input [INFILE] ... [SETTING] [VALUE] ... [FLAGS]\n\n"
            "Settings:\n"
            "--input, -i         The LAMMPS input file (required).\n"
            "--directory, -d     The calculation directory LAMMPS was run in.\n"
            "                    By default, this will be inferred from the parent\n"
            "                    directory of the LAMMPS input file.\n"
            "--skip, -s          Skip sum number of LAMMPS runs, i.e. skip an NVT\n"
            "                    part of the simulation. Default is 1.\n\n"
            "--stride, -j        Skip time steps with some stride.\n"
            "Flags:\n"
            "--verbose, -v       Print info.\n";
    }

}


/* UTILITIES */
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

std::vector<str> search_file(str fname, str match, int skip, str delimiter) {

    std::fstream instream(fname);
    str line;
    std::getline(instream, line);

    for (int irun = 0; irun <= skip; irun++) {
        while (line.find(match) == str::npos) {
            if (instream.eof()) return std::vector<str>();
            std::getline(instream, line);
        }
        if (irun < skip) std::getline(instream, line);
    }
    std::vector<str> tokens = split(line, delimiter);
    return tokens;
}

// Infer directory relative to the infile
str get_parent(const str fname) {
    if (fname.empty()) return str(".");
    if (fname == "/") return str("/");

    str without_hanger = fname;
    if (fname.ends_with(os_sep)) {
        without_hanger = fname.substr(0, fname.length()-os_sep.length());
    }

    std::vector<str> parts = split(without_hanger, os_sep);
    if (parts.size() < 2) return str(".");
    str directory = "";
    for (int i=0; i < parts.size()-1; i++) {
        directory += parts[i];
        if (i < parts.size()-2) directory += os_sep;
    }
    return directory;
}

}
