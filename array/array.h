#pragma once

#include <vector>

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

    int rows() const { return h; }
    int cols() const { return w; }

  private:
    int h, w;
};


}
