#include <cmath>

#include "parse.h"
#include "array.h"

// For now, tests in main.
void test_lammps_reader() {

    parse::LammpsReader reader("data", "data/lammps.in");

    array::Arr2<double> a1(3, 5);
    a1(2, 5) = 234.234;

    std::fstream short_data("data/short.dat");
    std::fstream vel("data/velocities.dat");

    // vel << short_data;
}

extern "C" const char *hello(int a) {
    return "Hello";
}

extern "C" const char *foo(const char* dir, float a, float b, float c, float d )
{
    printf("%s\n", dir);
    printf("%f\n", a);
    printf("%f\n", b);
    printf("%f\n", c);
    printf("%f\n", d);

    return "hello\n";
}

int main(int argc, char *argv[]) {

    test_lammps_reader();

    return 0;
}
