#include "parse.h"

#include <stdexcept>

int main() {
    int status = 0;

    int argc = 3;
    char *argv[] = {
        (char*)"mdcorr",
        (char*)"--input",
        (char*)"/path/that/doesnt/exist/asdlfkj23598wfejwaofj",
        NULL
    };

    parse::CLIReader cli(argc, argv);
    // This shouldn't hang
    try { parse::LammpsReader input(cli.args); }
    catch (std::invalid_argument const& ex) {}

    return 0;
}

