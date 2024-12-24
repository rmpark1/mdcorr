#include "parse.h"

#include <iostream>
#include <fstream>

int main(void) {

    // Check if there is data to test.
    str path = "data/1117dl_short.dat";
    std::fstream f(path);

    if (!f.good()) {
        std::cout << "No data for read test" << std::endl;
        return 0;
    }

    parse::LammpsSettings ls{
        "data", str("data/1117dl_short.in"), 0, 1, 10000000, 1
    };
    parse::LammpsReader data(ls);

    A3 velocities(1000, 100, 3);

    // data.load_range(velocities)

    // parse::read_step(path, velocities, 2, 0, 100);
    // return 0;
}
