#pragma once

#include <iostream>
#include <ranges>
#include <vector>

// namespace views = std::views;

namespace array {

/**
 * A continguous 3D array.
 */
template<class T>
class Arr3 : public std::vector<T> {

  public:

    int h, w, d;

    Arr3() : std::vector<T>(), h(0), w(0), d(0) {}

    Arr3(int h, int w, int d) : std::vector<T>(h*w*d), h(h), w(w), d(d) {}

    T operator()(int i, int j, int k) const {
        return std::vector<T>::operator[](i + j*h + k*(h*w)); }

    T &operator()(int i, int j, int k) {
        return std::vector<T>::operator[](i + j*h + k*(h*w)); }

    void free() { this->clear(); }

    void resize_contiguous(int x, int y, int z) {
        // Naive approach for now

        Arr3<T> temp(x,y,z);
        for (int i = 0; i < x; i++) {
            for (int j=0; j < y; j++) {
                for (int k=0; k < z; k++) {
                    if ((x > h) | (y > w) | (z > d)) { temp(i,j,k) = T(); }
                    else { temp(i,j,k) = (*this)(i,j,k); }
                }
            }
        }

        this->resize(h*w*d);

        for (int i = 0; i < x; i++) {
            for (int j=0; j < y; j++) {
                for (int k=0; k < z; k++) {
                    (*this)(i,j,k) = temp(i,j,k);
                }
            }
        }
    }


};


}
