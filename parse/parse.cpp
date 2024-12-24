#include "parse.h"

const str CLI_DOC = "Usage:\n"
                    "mdcorr --input [LOGFILE] ... [SETTING] [VALUE] ... [FLAGS]\n\n"
                    "Settings:\n"
                    "--input, -i         The LAMMPS input file (required).\n"
                    "--directory, -d     The calculation directory LAMMPS was run in.\n"
                    "                    By default, this will be inferred from the parent\n"
                    "                    directory of the LAMMPS input file.\n"
                    "--skip, -s          Skip sum number of LAMMPS runs, i.e. skip an NVT\n"
                    "                    part of the simulation. Default is 1.\n"
                    "--stride, -j        Skip time steps with some stride.\n"
                    "--steps, -t         Specify a cap on the number of time steps.\n"
                    "--mem, -m           Specify chunk size in kB.\n"
                    "--atoms, -c         Specify max number of atoms to average over.\n\n"
                    "Flags:\n"
                    "--verbose, -v       Print info.\n";
                    // "--fft, -f           Use FFT implementation (on by default).\n"

str os_sep =
#ifdef _WIN32
"\\";
#else
"/";
#endif

#define MIN_SHIFT 100
#define SHIFT_MOD 0.001

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
        timesteps(args.timesteps),
        directory(args.directory) {

    if (directory.empty()) directory = get_parent(args.input);

    parse_log(args);

    // check_dump();

    if (verbose) print_summary();
}

void LammpsReader::parse_log(LammpsSettings args) {

    std::vector<std::vector<str> > tokens = search_file(args.input, "run");
    int n_sim_steps = std::stoi(tokens[skip][1]);

    tokens = search_file(args.input, "dump");
    std::vector<str> last_dump = tokens[tokens.size()-1];
    dump_stride = std::stoi(last_dump[4]);
    dump_path = directory+os_sep+last_dump[5];

    // Read number of species and number of each species
    tokens = search_file(args.input, "create_atoms");
    nspecies = tokens.size();
    natoms = 0;
    for (auto tok : tokens) natoms += std::stoi(tok[3]);
    nsteps = (n_sim_steps / dump_stride) + 1;
}


void LammpsReader::print_summary() {

    printf("----------------\n");
    printf("Reading LAMMPS input\n");
    printf("----------------\n");
    printf("Calculation directory: %s\n", directory.c_str());
    printf("# of species: %i\n", nspecies);
    printf("# of atoms: %i\n", natoms);
    printf("# of dump steps: %i\n", nsteps);
    printf("LAMMPS dump stride: %i\n", dump_stride);
    printf("# of correlation points: %i\n", nsteps/stride);
    printf("----------------\n");
    fflush(stdout);
}

std::fstream LammpsReader::get_fstream() {
    return std::fstream(dump_path);
}

void LammpsReader::check_dump() {

    // Read first header
    std::fstream f(dump_path);
    str line;
    std::getline(f, line);

    if (line.find(str("TIMESTEP")) == str::npos) {
        throw std::runtime_error("Invalid dump file.");
    }

    int first_timestep, last_timestep;
    f >> first_timestep;

    // Go to the end
    f.seekg(-1, std::ios::end);
    char c;

    unsigned long size = f.tellg();
    size++;

    auto rget = [&f, &c]{
        f.seekg(-2, std::ios::cur);
        c = f.get();
    };

    str check("TIMESTEP");

    while (true) {
        rget();
        if (c == 'P') {
            str match("");
            for (auto l : check) {
                match.insert(0, 1, c);
                rget();
            }
            if (match == check) break;
        }
    }

    std::getline(f, line);
    f >> last_timestep;

    int found = (last_timestep - first_timestep) / dump_stride + 1;

    // If unexpected number of steps found, then copy into new array
    if (found != nsteps) {
        if (verbose) std::cout << "WARNING: Found less data than input specified "
            << found << "/" << nsteps << std::endl;
        nsteps = found;
    }

    avg_size = static_cast<double>(size)/found;

}


/* Load in a single time step of LAMMPS data. */
void LammpsReader::load_step(A3 &velocities, int step, int min_atom, int max_atom) {

    unsigned long pos = find_step(step);

    std::fstream f(dump_path);
    f.seekg(pos, std::ios::beg);
}

unsigned long LammpsReader::find_step(int step) {

    std::fstream f(dump_path);
    // Move pointer to guess, find the nearest timestep
    double guess = static_cast<int>(avg_size*(static_cast<double>(step)/nsteps));
    double shift = fmin(MIN_SHIFT, avg_size*SHIFT_MOD);
    f.seekg(guess-shift, std::ios::beg);

    char c;
    auto rget = [&f, &c]{
        f.seekg(-2, std::ios::cur);
        c = f.get(); };

    str check("TIMESTEP");

    int cur_step;

    unsigned long pos = f.tellg();

    return pos;

    // // Try forward
    // while (true) {
    //     rget();
    // }

    // // Try backward.
    // f.seekg(guess-shift, std::ios::beg);

    // rget();
    // if (c == 'P') {
    //     str match("");
    //     for (auto l : check) {
    //         match.insert(0, 1, c);
    //         rget();
    //     }
    //     if (match == check) break;
    // }


}


/**
 * Read all data in a specified directory
 */
int LammpsReader::load(A3 &velocities) {

    if(verbose) std::cout << "Reading " << dump_path << std::endl;
    const auto start = timer::now();

    // Load entire range
    int found = load_range(velocities, 0, natoms);

    const auto finish = timer::now();
    if (verbose) std::cout << "Finished reading in "
        << chrono::duration_cast<chrono::minutes>(finish - start) << std::endl;
    return found;
}

int LammpsReader::load_range(A3 &velocities, int min_atom, int max_atom) {

    double vx, vy, vz;
    int id;
    str line;
    int nsteps_found = 0;
    int tstep;
    int progress;

    std::fstream file_handle(dump_path);
    if (file_handle.fail()) {
        throw std::invalid_argument(str("ERROR: Dump file not found -- ")+dump_path);
    }

    const auto start = timer::now();
    std::getline(file_handle, line);
    while (!file_handle.eof()) {

        // Skip header
        for (int i=0; i<8; i++) std::getline(file_handle, line);

        for (int i=0; i < natoms; i++) {
            file_handle >> id >> vx >> vy >> vz;
            tstep = nsteps_found/stride;
            if ((nsteps_found % stride == 0)
              & (id-1 < max_atom)
              & (id-1 >= min_atom)
              ) {
                velocities(tstep, id-1-min_atom, 0) = vx;
                velocities(tstep, id-1-min_atom, 1) = vy;
                velocities(tstep, id-1-min_atom, 2) = vz;
            }
        }

        
        nsteps_found++;

        std::getline(file_handle, line);
        std::getline(file_handle, line);

        // if (verbose & (nsteps_found % 100 == 0)) {
        //     std::cout << nsteps_found << std::endl;
        // }

        if (nsteps_found >= nsteps) break;
        if (nsteps_found >= timesteps) break;
    }

    return nsteps_found/stride;
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
    args.timesteps = -1;

    help = 0;
    fft = 1;
    mem = 0;
    max_atoms = -1;


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
        if (match("--help", "-h")) { help = 1; remaining -= 1; continue; }

        arg_val = str(argv[argc-remaining+1]);
        if (match("--input", "-i")) { args.input = str(arg_val); remaining -= 2; }
        if (match("--directory", "-d")) { args.directory = str(arg_val); remaining -= 2; }
        if (match("--skip", "-s")) { args.skip = std::stoi(arg_val); remaining -= 2; }
        if (match("--stride", "-j")) { args.stride = std::stoi(arg_val); remaining -= 2; }
        if (match("--steps", "-t")) { args.timesteps = std::stoi(arg_val); remaining -= 2; }
        if (match("--mem", "-m")) { mem = std::stoi(arg_val); remaining -= 2; }
        if (match("--fft", "-f")) { fft = std::stoi(arg_val); remaining -= 2; }
        if (match("--atoms", "-a")) { max_atoms = std::stoi(arg_val); remaining -= 2; }
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
