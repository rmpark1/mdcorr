#include "array.h"

typedef array::Arr3<int> A3;

typedef std::string str;

// int main() {
//
//     // Test functional interface, loop over various dimensions.
//     A3 a(3, 2, 1);
//     view::iota = i1;
//     for (int i :
//              view::iota(0, 6)
//            | view::filter(even)
//            | view::transform(square))
//         std::cout << i << ' ';
//
//     return 0;
// }

// #include <iostream>
// #include <ranges>
 
int main()
{
    auto even = [](int i) { return 0 == i % 2; };
    auto square = [](int i) { return i * i; };
 
    for (int i : std::views::iota(0, 6)
               | std::views::filter(even)
               | std::views::transform(square))
        std::cout << i << ' ';
    std::cout << '\n';
}
