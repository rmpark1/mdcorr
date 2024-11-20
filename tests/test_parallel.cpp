// #include <algorithm>
// #include <chrono>
// #include <numeric>
// #include <execution>
// #include <cmath>
// #include <vector>
// #include <iostream>
//
// namespace execution = std::execution;
//
// double complex_map(double t) {
//     if (t > 3) { return t; }
//     return t*t;
// }
//
// void run_map(int N) {
//
//     auto inline_map = [](int &i){ return i*log(i+1); };
//
//     std::vector<double> a(N);
//
//     std::iota(a.begin(), a.end(), 0);
//
//     auto return = std::for_each(a.begin(), a.end(), inline_map);
//
// }
//
// int main() {
//
//     run_map(10);
//
//     return 0;
// }
#include <algorithm>
#include <string>
#include <chrono>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <random>
#include <vector>
 
typedef std::string str;

#ifdef PARALLEL
#include <execution>
    namespace execution = std::execution;
#else
    enum class execution { seq, unseq, par_unseq, par };
#endif
 
void measure([[maybe_unused]] auto policy, std::vector<std::uint64_t> v)
{
    const auto start = std::chrono::steady_clock::now();
#ifdef PARALLEL
    std::sort(policy, v.begin(), v.end());
#else
    std::sort(v.begin(), v.end());
#endif
    const auto finish = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count()
              << '\n';
};
 
int main()
{
    std::vector<std::uint64_t> v(1'000'000);
    std::mt19937 gen {std::random_device{}()};
    std::ranges::generate(v, gen);

    #ifdef PARALLEL
    std::cout << "PARALLEL ON\n";
    #else
    std::cout << "PARALLEL OFF\n";
    #endif
 
    measure(execution::seq, v);
    measure(execution::unseq, v);
    measure(execution::par_unseq, v);
    measure(execution::par, v);
}
// error: invalid operands to binary expression
// ('std::ostream' (aka 'basic_ostream<char>')
//  and
//  'typename enable_if<__is_duration<duration<long long, ratio<1, 1000>>>::value, duration<long long, ratio<1, 1000>>>::type'
//  (aka 'std::chrono::duration<long long, std::ratio<1, 1000>>'))
