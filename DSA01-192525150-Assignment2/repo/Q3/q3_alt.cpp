// Alternative 1: one Parcel with optional (default) fields.  Alternative 2: one function template.
#include <chrono>
#include <iomanip>
#include <iostream>
#include <type_traits>
struct Parcel { double kg; bool express = false; bool insured = false; };
static Parcel fromGrams(int g, bool e = false, bool i = false) { return Parcel{g / 1000.0, e, i}; }
__attribute__((noinline)) double chargeParcel(const Parcel& p)
{
    double b = 40.0 * p.kg; if (p.express) b += b * 0.5; if (p.insured) b += 25.0; return b;
}
template <class W>
__attribute__((noinline)) double chargeT(W w, bool e = false, bool i = false)
{
    double kg = std::is_integral<W>::value ? static_cast<double>(w) / 1000.0 : static_cast<double>(w);
    double b = 40.0 * kg; if (e) b += b * 0.5; if (i) b += 25.0; return b;
}
__attribute__((noinline)) double chargeOver(double kg, bool e = false, bool i = false)
{
    double b = 40.0 * kg; if (e) b += b * 0.5; if (i) b += 25.0; return b;
}
template <class F> double timeit(F f) { double best = 1e18; for (int r = 0; r < 5; ++r) { auto t0 = std::chrono::steady_clock::now(); f(); auto t1 = std::chrono::steady_clock::now(); double ms = std::chrono::duration<double, std::milli>(t1 - t0).count(); if (ms < best) best = ms; } return best; }
int main()
{
    std::cout << std::fixed << std::setprecision(2);
    std::cout << chargeParcel(Parcel{2.5}) << ' ' << chargeParcel(fromGrams(1500, true)) << ' ' << chargeParcel(Parcel{3.0, true, true}) << '\n';
    std::cout << chargeT(2.5) << ' ' << chargeT(1500, true) << ' ' << chargeT(1.5, true, true) << '\n';
    std::cout << "sizeof(Parcel)=" << sizeof(Parcel) << " sizeof(double)=" << sizeof(double) << " sizeof(bool)=" << sizeof(bool) << '\n';
    const int N = 50000000; volatile double sink = 0;
    std::cout << "overloads+defaults : " << timeit([&]{ double a=0; for (int k=0;k<N;++k) a+=chargeOver(1.0+(k&7), k&1, k&2); sink=a; }) << " ms\n";
    std::cout << "Parcel (const ref) : " << timeit([&]{ double a=0; for (int k=0;k<N;++k) a+=chargeParcel(Parcel{1.0+(k&7), bool(k&1), bool(k&2)}); sink=a; }) << " ms\n";
    std::cout << "function template  : " << timeit([&]{ double a=0; for (int k=0;k<N;++k) a+=chargeT(1.0+(k&7), k&1, k&2); sink=a; }) << " ms\n";
}
