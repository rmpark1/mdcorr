#include "parse.h"



int test_get_parent() {
    int status = 0;

    str path, parent;

    std::vector<str> path_tests {
        "/path/with/root", "/path/with",
        "relative/path", "relative",
        "/", "/",
        "/path/with/hanger/", "/path/with",
        "", ".",
        "file", ".",
    };

    for (int i=0; i<path_tests.size(); i+=2) {
        parent = parse::get_parent(path_tests[i]);
        if (parent != path_tests[i+1]) status++;
        std::cout << path_tests[i+1] << "<<\n";
        std::cout << parent << "<<\n";
        std::cout << status << "<<\n";
    }


    return status;
}




int main() {

    int status = 0;
    status += test_get_parent();
    return status;
}
