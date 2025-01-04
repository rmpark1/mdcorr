#pragma once

#include <iostream>
#include <iterator>
#include <vector>

typedef std::size_t size_t; // For linear scaling
typedef unsigned short uns; // For log scaling

namespace array {

/**
 * A continguous 3D array.
 */
template<class T>
class Arr3 : public std::vector<T> {

  public:

    size_t h, w, d;

    Arr3() : std::vector<T>(), h(0), w(0), d(0) {}

    Arr3(size_t h, size_t w, size_t d) : std::vector<T>(h*w*d), h(h), w(w), d(d) {}

    T operator()(size_t i, size_t j, size_t k) const {
        return std::vector<T>::operator[](i + j*h + k*(h*w)); }

    T &operator()(size_t i, size_t j, size_t k) {
        return std::vector<T>::operator[](i + j*h + k*(h*w)); }

    typename std::vector<T>::iterator begin_(size_t j, size_t k) {
        return this->begin()+j*h+k*h*w;
    }
    typename std::vector<T>::reverse_iterator rbegin_(size_t j, size_t k) {
        return this->rbegin() + (h*w*d - j*h + k*h*w - h);
    }

    void free() { this->clear(); }

    void arange() {
        size_t c = 0;
        for (size_t k = 0; k < d; k++) {
            for (size_t j = 0; j < w; j++) {
                for (size_t i = 0; i < h; i++) {
                    (*this)(i, j, k) = c;
                    c++;
                }
            }
        }
    }

    void fill_range(size_t imin, size_t imax, T value) {
        for (size_t k = 0; k < d; k++) {
            for (size_t j = 0; j < w; j++) {
                for (size_t i = imin; i < imax; i++) {
                    (*this)(i, j, k) = value;
                }
            }
        }
    }

    void resize_contiguous(size_t x, size_t y, size_t z) {

        Arr3<T> temp(x,y,z);
        for (size_t i=0; i < x; i++) {
            for (size_t j=0; j < y; j++) {
                for (size_t k=0; k < z; k++) {
                    if ((i > h-1) | (j > w-1) | (k > d-1)) temp(i,j,k) = T();
                    else temp(i,j,k) = (*this)(i,j,k);
                }
            }
        }

        this->resize(x*y*z);
        h = x;
        w = y;
        d = z;

        for (size_t i = 0; i < x; i++) {
            for (size_t j=0; j < y; j++) {
                for (size_t k=0; k < z; k++) {
                    (*this)(i,j,k) = temp(i,j,k);
                }
            }
        }
    }
};

}
