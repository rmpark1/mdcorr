#include "parse.h"

const char os_sep =
#ifdef _WIN32
'\\';
#else
'/';
#endif

namespace parse {
/**
 * Lammps Reader that will find atom dump files in a ceratin directory>
 *
 * @param directory the root path containing dump files.
 * @param file_fmt a filetype suffix to match dump files and filter out nonrelevant data.
 */

LammpsReader::LammpsReader(str directory, str lammps_out, str suffix) {
    // Look for data files.
    find_dump_files(directory, suffix);
    // Infer nsteps from output file
    get_nsteps(lammps_out);
}


/**
 * Lammps Reader that will find atom dump files in a ceratin directory>
 *
 * @param directory the root path containing dump files.
 * @param file_fmt a filetype suffix to match dump files and filter out nonrelevant data.
 */
LammpsReader::LammpsReader(str directory, int nsteps_, str suffix)
        : nsteps(nsteps_) {

    // Look for data files.
    find_dump_files(directory, suffix);
}

LammpsReader::~LammpsReader() { }

void LammpsReader::find_dump_files(str directory, str suffix) {

    DIR *base = opendir(directory.c_str());
    struct dirent *dp;

    dp=readdir(base);
    dp=readdir(base);

    str path_string;
    while ( (dp=readdir(base)) )  {
        path_string = directory + os_sep + str(dp->d_name);
        if (path_string.ends_with(suffix)) {
            paths.push_back(path_string);
        }
    }

    closedir(base);
}

/**
 * Add a dump file path manually for parsing.
 */
int LammpsReader::add_file(str fname) {

    if (!access(fname.c_str(), F_OK)) {
        if (verbose) printf("File does not exist: %s", fname.c_str());
        return 1;
    }

    paths.push_back(fname);
    return 0;
}

/**
 * Read all data in a specified directory
 */
int LammpsReader::load() {
    // Open all file streams
    for (auto &fname : paths) {
        std::fstream file_handle(fname);
        // data_stream << file_handle;
        if(verbose) std::cout << "reading " << fname << "\n";
    }
    return 0;
}

int LammpsReader::check_dimensions() {
    return 0;
}
    

}
