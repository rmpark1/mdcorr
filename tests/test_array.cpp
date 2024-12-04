#include <cmath>

#include "array.h"


typedef array::Arr3<int> A3;
typedef std::string str;

int check_overlap(int h, int w, int d) {

    A3 arr(h, w, d);
    for (int i=0; i < arr.h; i++) {
        for (int j=0; j < arr.w; j++) {
            for (int k=0; k < arr.d; k++) {
                arr(i,j,k) = 0.0;
            }
        }
    }

    for (int i=0; i < arr.h; i++) {
        for (int j=0; j < arr.d; j++) {
            for (int k=0; k < arr.w; k++) {
                arr(i,k,j) = arr(i,k,j) + 1.0;
            }
        }
    }

    for (int i=0; i < arr.d; i++) {
        for (int j=0; j < arr.w; j++) {
            for (int k=0; k < arr.h; k++) {
                if (arr(k,j,i) != 1.0) return 1;
            }
        }
    }
    return 0;
}

int test_resize_change(A3 &arr, int x, int y, int z) {
    int x0 = arr.h;
    int y0 = arr.w;
    int z0 = arr.d;
    for (int i=0; i < arr.h; i++) {
        for (int j=0; j < arr.w; j++) {
            for (int k=0; k < arr.d; k++) {
                str s = std::to_string(i) + std::to_string(j) + std::to_string(k);
                arr(i,j,k) = (double)std::stoi(s);
            }
        }
    }

    arr.resize_contiguous(x, y, z);

    for (int i=0; i < fmin(x0, x); i++) {
        for (int j=0; j < fmin(y0, y); j++) {
            for (int k=0; k < fmin(z0, z); k++) {
                str s = std::to_string(i) + std::to_string(j) + std::to_string(k);
                if (arr(i,j,k) != (double)std::stoi(s)) return 1;
            }
        }
    }
    return 0;
}

int test_resize() {

    int status = 0;
    A3 a(2, 3, 1);
    if (test_resize_change(a, 4, 2, 2)) status = 1;
    if (test_resize_change(a, 3, 4, 2)) status = 1;
    if (test_resize_change(a, 2, 4, 4)) status = 2;
    if (test_resize_change(a, 2, 2, 4)) status = 3;
    if (test_resize_change(a, 20, 20, 20)) status = 4;
    if (test_resize_change(a, 1, 1, 1)) status = 5;
    if (test_resize_change(a, 150, 760, 2)) status = 6;
    if (test_resize_change(a, 100, 160, 3)) status = 7;

    std::cout << status << std::endl;

    return status;
}

int main() {
    int status = 0;

    if (check_overlap(1, 1, 1)) status = 1;
    if (check_overlap(10, 10, 10)) status = 2;
    if (check_overlap(100, 1, 1)) status = 3;
    if (check_overlap(1, 100, 1)) status = 4;
    if (check_overlap(1, 100, 100)) status = 5;
    if (check_overlap(1, 1, 100)) status = 6;

    if (test_resize()) status = 7;

    std::cout << status << std::endl;
    return status;
}

