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

    void free() { this->clear(); }


    void resize_contiguous(int x, int y, int z) {
        // Naive approach for now

        // X dimension
        // Add if larger size
        for (int k = d-1; k >= 0; k--) {
            for (int j = w-1; j >= 0; j--) {
                for (int i = h; i < x; i++) {
                    this->insert(this->begin()+(i + j*h + k*h*w), 0);
                }
            }
        }
        // Subtract if smaller size
        for (int k = d-1; k >= 0; k--) {
            for (int j = w-1; j >= 0; j--) {
                for (int i = h-1; i >= x; i--) {
                    this->erase(this->begin()+(i + j*h + k*h*w));
                }
            }
        }
        h = x;
        // Y dimension
        // Add if larger size
        for (int k = d-1; k >= 0; k--) {
            for (int j = w; j < y; j++) {
                for (int i = 0; i < h; i++) {
                    this->insert(this->begin()+(i + j*h + k*h*w), 0);
                }
            }
        }
        // Subtract if smaller size
        for (int k = d-1; k >= 0; k--) {
            for (int j = w-1; j >= y; j--) {
                for (int i = h-1; i >= 0 ; i--) {
                    this->erase(this->begin()+(i + j*h + k*h*w));
                }
            }
        }
        w = y;

        // Z dimension
        // Add if larger size
        for (int k = d; k < z; k++) {
            for (int j = 0; j < w; j++) {
                for (int i = 0; i < h; i++) {
                    this->insert(this->begin()+(i + j*h + k*h*w), 0);
                }
            }
        }
        // Subtract if smaller size
        for (int k = d-1; k >= z; k--) {
            for (int j = w-1; j >= 0; j--) {
                for (int i = h-1; i >=0; i--) {
                    this->erase(this->begin()+(i + j*h + k*h*w));
                }
            }
        }
        d = z;

    }
};



}
