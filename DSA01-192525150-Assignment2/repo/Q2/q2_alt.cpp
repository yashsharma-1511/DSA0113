#include <chrono>
#include <iostream>
#include <vector>
#include <random>
int& highestRef(std::vector<int>& v) { size_t b = 0; for (size_t i = 1; i < v.size(); ++i) if (v[i] > v[b]) b = i; return v[b]; }
int highestIndex(const std::vector<int>& v) { size_t b = 0; for (size_t i = 1; i < v.size(); ++i) if (v[i] > v[b]) b = i; return (int)b; }
int* highestPtr(std::vector<int>& v) { size_t b = 0; for (size_t i = 1; i < v.size(); ++i) if (v[i] > v[b]) b = i; return &v[b]; }
template <class F> double timeit(F f) { double best = 1e18; for (int r = 0; r < 5; ++r) { auto t0 = std::chrono::steady_clock::now(); f(); auto t1 = std::chrono::steady_clock::now(); double ms = std::chrono::duration<double, std::milli>(t1 - t0).count(); if (ms < best) best = ms; } return best; }
int main()
{
    // calling code for the three styles
    std::vector<int> m = {56, 71, 88, 64};
    highestRef(m) += 2;           std::cout << "ref   : m[2]=" << m[2] << '\n';
    m[highestIndex(m)] += 2;      std::cout << "index : m[2]=" << m[2] << '\n';
    *highestPtr(m) += 2;          std::cout << "ptr   : m[2]=" << m[2] << '\n';
    // growth: which handles stay valid
    int& r = highestRef(m); int* p = highestPtr(m); int ix = highestIndex(m);
    for (int i = 0; i < 100; ++i) m.push_back(1);
    std::cout << "after growth: ref valid? " << std::boolalpha << (&r == &m[2]) << ", ptr valid? " << (p == &m[2]) << ", index valid? " << (m[ix] == 94) << '\n';
    std::cout << "n          ref(ms)   index(ms)  ptr(ms)\n";
    std::mt19937 g(1);
    for (int n : {1000, 100000, 1000000, 10000000}) {
        std::vector<int> v(n); for (auto& x : v) x = g() % 1000000;
        volatile long long sink = 0;
        int reps = n <= 100000 ? 1000 : 1; if (n <= 1000) reps = 100000;
        double a = timeit([&]{ for (int k=0;k<reps;++k) sink += highestRef(v); }) / reps;
        double b = timeit([&]{ for (int k=0;k<reps;++k) sink += highestIndex(v); }) / reps;
        double c = timeit([&]{ for (int k=0;k<reps;++k) sink += *highestPtr(v); }) / reps;
        std::cout << n << "   " << a << "   " << b << "   " << c << '\n';
    }
}
