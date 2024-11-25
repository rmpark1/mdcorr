#include "correlate.h"
#include "array.h"

#include <cmath>

typedef array::Arr3<double> A3;

int main() {

    // 1D correlation first
    int N = 8;
    A3 a(N,1,1);
    A3 b(N,1,1);

    for (int i=0; i < N; i++) {
        a(i,0,0) = i*.1;
    }
    corr::autocorrelate(a, b, 0);

    for (int i=0; i < N; i++) {
        std::cout << b(i,0,0) << "\n";
    }
}
