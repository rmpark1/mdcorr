#include "parse.h"

const str CLI_DOC = "Usage:\n"
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
                    "--fft, -f           Use FFT implementation (on by default).\n"
                    "--verbose, -v       Print info.\n";

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
LammpsReader::LammpsReader(LammpsSettings args) :
        verbose(args.verbose),
        skip(args.skip),
        stride(args.stride),
        directory(args.directory) {

    if (directory.empty()) directory = get_parent(args.input);

    std::vector<std::vector<str> > tokens = search_file(args.input, "run");

    int n_sim_steps = std::stoi(tokens[args.skip][1]);

    tokens = search_file(args.input, "dump");
    std::vector<str> last_dump = tokens[tokens.size()-1];
    int dump_stride = std::stoi(last_dump[4]);
    str dump_path = last_dump[5];
    paths.push_back(dump_path);

    // Read number of species and number of each species
    tokens = search_file(args.input, "create_atoms");
    int nspecies = tokens.size();
    natoms = 0;
    for (auto tok : tokens) {
        natoms += std::stoi(tok[3]);
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
        fflush(stdout);
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
    int tstep;
    int progress;

    // Open all file streams
    for (auto &fname : paths) {

        str full_path = directory+os_sep+fname;
        std::fstream file_handle(full_path);
        if (file_handle.fail()) {
            throw std::invalid_argument(str("ERROR: Dump file not found -- ")+full_path);
        }
        if(verbose) std::cout << "Reading " << full_path << " ... " << std::flush;

        const auto start = timer::now();
        std::getline(file_handle, line);
        while (!file_handle.eof()) {

            for (int i=0; i<8; i++) std::getline(file_handle, line);

            for (int i=0; i< natoms; i++) {
                file_handle >> id >> vx >> vy >> vz;
                tstep = nsteps_found/stride;
                if (nsteps_found % stride == 0) {
                    velocities(tstep, id-1, 0) = vx;
                    velocities(tstep, id-1, 1) = vy;
                    velocities(tstep, id-1, 2) = vz;
                }
            }
            
            nsteps_found++;

            std::getline(file_handle, line);
            std::getline(file_handle, line);

            if (verbose & (nsteps_found % 100 == 0)) {
                std::cout << nsteps_found << std::endl;
            }

            if (nsteps_found >= nsteps) break;
        }

        const auto finish = timer::now();
        if (verbose) std::cout << "finished in "
            << chrono::duration_cast<chrono::minutes>(finish - start) << std::endl;

    }

    return nsteps_found/stride;
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

    args.directory = "";
    args.input = "";
    args.skip = 1;
    args.stride = 1;
    args.verbose = 0;

    help = 0;
    fft = 1;

    read_args(argc, argv);
    if (help) std::cout << CLI_DOC; return;

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

        if (match("--verbose", "-v")) { args.verbose = 1; remaining -= 1; continue; }
        if (match("--fft", "-f")) { fft = 1; remaining -= 1; continue; }
        if (match("--help", "-h")) { help = 1; remaining -= 1; continue; }

        arg_val = str(argv[argc-remaining+1]);
        if (match("--input", "-i")) { args.input = str(arg_val); remaining -= 2; }
        if (match("--directory", "-d")) { args.directory = str(arg_val); remaining -= 2; }
        if (match("--skip", "-s")) { args.skip = std::stoi(arg_val); remaining -= 2; }
        if (match("--stride", "-j")) { args.stride = std::stoi(arg_val); remaining -= 2; }
    }
}

void CLIReader::check_input() {

    bool invalid = false;
    if (args.input.empty()) {
            printf("Missing input file argument: '--input <lammps_input>'\n");
            invalid = true;
    }
    if (invalid) std::cout << CLI_DOC;
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

// Read lammps log file and find keyword specifications
std::vector<std::vector<str> > search_file(str fname, str match) {

    std::fstream instream(fname);
    if (instream.fail())
        throw std::invalid_argument(str("ERROR: File not found -- ") + fname);

    int line_number = 0;
    auto getline = [&instream, &line_number](str &line) {
        std::getline(instream, line);
        line_number++;
    };

    str line;
    getline(line);

    std::vector<std::vector<str> > token_list;

    // Replace whitespace with spaces to get common delimiter
    std::regex white("\\s+");

    while (!instream.eof()) {

        while (!line.starts_with(match) & !instream.eof()) getline(line);
        if (instream.eof() & !line.starts_with(match)) break;

        // At line with correct keyword, loop until ${variable} expressions have been evaluated.
        while (line.find("${") != str::npos) getline(line);

        // Make sure this is the same keyword,
        // throw error if, e.g., an input file with ${} expressions has been passed.
        if (line.find(match) == str::npos) {
            str msg = "Input file has unevaluated ${variable} expressions on line "
                + std::to_string(line_number) + "\n";
            throw std::invalid_argument(msg);
        }

        str space = std::regex_replace(line, white, " ");
        std::vector<str> split_str = split(space, " ");
        token_list.push_back(split_str);

        getline(line);
    }
 
    if (token_list.size() == 0) {
        throw std::invalid_argument(str("LAMMPS file does not have a '"
            + match + str("' statement(s)")));
    }

    return token_list;
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
