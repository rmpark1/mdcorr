#include "correlate.h"
#include "array.h"

#include <cmath>

typedef array::Arr3<double> A3;

int main() {

    // 1D correlation first
    int N = 13;
    A3 a(N,1,1);

    for (int i=0; i < N; i++) {
        a(i,0,0) = i;
    }

    for (int i : a) {
        std::cout << i;
    }
    std::cout << std::endl;

    A3 out(N,1,1);
    corr::autocorrelate_direct(a);
    for (double i : out) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    corr::autocorrelate(a);

    return 0;

}
