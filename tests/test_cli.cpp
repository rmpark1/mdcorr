#include "parse.h"

int main() {
    int status = 0;

    int argc = 3;
    char *argv[] = {
        (char*)"mdcorr",
        (char*)"--input",
        (char*)"hello",
        NULL
    };

    parse::CLIReader input(argc, argv);

    if (!input.directory.empty()) return 1;
    if (input.input != "hello") return 2;

    argc = 10;
    char *argv2[] = {
        (char*)"mdcorr",
        (char*)"--input",
        (char*)"hello",
        (char*)"--verbose",
        (char*)"-d",
        (char*)"path/to/dir",
        (char*)"--skip",
        (char*)"3",
        (char*)"--stride",
        (char*)"10",
        NULL
    };

    parse::CLIReader input2(argc, (char **)argv2);

    if (input2.directory != "path/to/dir") return 3;
    if (input2.input != "hello") return 4;
    if (input2.skip != 3) return 5;
    if (!input2.verbose) return 6;
    if (input2.stride != 10) return 7;

    return 0;

}

