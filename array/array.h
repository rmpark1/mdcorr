#pragma once

#include <iostream>
#include <iterator>
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

    typename std::vector<T>::iterator begin_(int j, int k) {
        return this->begin()+j*h+k*h*w;
    }
    typename std::vector<T>::reverse_iterator rbegin_(int j, int k) {
        return this->rbegin() + (h*w*d - j*h + k*h*w - h);
    }

    void free() { this->clear(); }

    void arange() {
        int c = 0;
        for (int k = 0; k < d; k++) {
            for (int j = 0; j < w; j++) {
                for (int i = 0; i < h; i++) {
                    (*this)(i, j, k) = c;
                    c++;
                }
            }
        }
    }

    void fill_range(int imin, int imax, T value) {
        for (int k = 0; k < d; k++) {
            for (int j = 0; j < w; j++) {
                for (int i = imin; i < imax; i++) {
                    (*this)(i, j, k) = value;
                }
            }
        }
    }

    void resize_contiguous(int x, int y, int z) {

        Arr3<T> temp(x,y,z);
        for (int i=0; i < x; i++) {
            for (int j=0; j < y; j++) {
                for (int k=0; k < z; k++) {
                    if ((i > h-1) | (j > w-1) | (k > d-1)) temp(i,j,k) = T();
                    else temp(i,j,k) = (*this)(i,j,k);
                }
            }
        }

        this->resize(x*y*z);
        h = x;
        w = y;
        d = z;

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
