#include "parse.h"

int main() {
    int s = 0;

    int argc = 3;
    char *argv[] = {
        (char*)"mdcorr",
        (char*)"--input",
        (char*)"hello",
        NULL
    };

    parse::CLIReader cli(argc, argv);

    parse::Input input = cli.args;

    if (!input.directory.empty()) s=1;
    if (input.input != "hello") s=2;

    argc = 11;
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
        (char*)"--help",
        NULL
    };

    parse::CLIReader cli2(argc, (char **)argv2);

    parse::Input input2 = cli2.args;


    if (input2.directory != "path/to/dir") s=3;
    if (input2.input != "hello") s=4;
    if (input2.skip != 3) s=5;
    if (!input2.verbose) s=6;
    if (input2.stride != 10) s=7;
    if (cli2.help != 1) s=8;

    std::cout << s << " Exit Code\n";

    return s;

}

