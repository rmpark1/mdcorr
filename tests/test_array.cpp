#include "array.h"

typedef array::Arr3<int> A3;

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

int main() {

    int err_sum = 0;
    err_sum += check_overlap(1, 1, 1);
    err_sum += check_overlap(10, 10, 10);
    err_sum += check_overlap(100, 1, 1);
    err_sum += check_overlap(1, 100, 1);
    err_sum += check_overlap(1, 100, 100);
    err_sum += check_overlap(1, 1, 100);
    return err_sum;
}

