#pragma once

#include <vector>
#include <functional>

typedef std::string str;

namespace array {

/**
 * A continguous 2D array.
 */
template<class T>
class Arr2 : private std::vector<T> {

  public:
    Arr2() : std::vector<T>(), h(0), w(0) {}

    Arr2(int h, int w) : std::vector<T>(h*w), h(h), w(w) {}

    T operator()(int i, int j) const 
    { return std::vector<T>::operator[](i+j*h); }

    T &operator()(int i, int j) 
    { return std::vector<T>::operator[](i+j*h); }

    void free() { this->clear(); }

  private:
    int h, w;
};

/**
 * A continguous 3D array.
 */
template<class T>
class Arr3 : private std::vector<T> {

  public:
    Arr3() : std::vector<T>(), h(0), w(0), d(0) {}

    Arr3(int h, int w, int d) : std::vector<T>(h*w*d), h(h), w(w), d(d) {}

    T operator()(int i, int j, int k) const {
        return std::vector<T>::operator[](i + j*h + k*(h*w)); }

    T &operator()(int i, int j, int k) {
        return std::vector<T>::operator[](i + j*h + k*(h*w)); }

    void free() { this->clear(); }

    int h, w, d;

};

}
