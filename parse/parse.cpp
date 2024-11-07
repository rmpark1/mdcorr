#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <string>

#include <dirent.h>

using fs = std::experimental::filesystem;
using walk = fs::recursive_directory_iterator;

namespace parse {

LammpsReader::LammpsReader(std::string directory, std::string file_fmt=".dat") {

    // Look for particle counts and request memory
    for (const auto& path : walk(directory))
        if (path.find(file_fmt) != std::string::npos) {
            fsys::path full_path = fsys::path(directory) + fsys::path(path);
            paths.push_back(full_path);
            std::cout << full_path << "\n";
    }

}

LammpsReader::~LammpsReader() {}

}
