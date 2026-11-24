// seat-counter cost only (no strings): static member vs global variable vs counter object
#include <chrono>
#include <iostream>
struct S { static int issued; static int next() { return issued < 3 ? ++issued : 0; } };
int S::issued = 0;
int g = 0; __attribute__((noinline)) int nextGlobal() { return g < 3 ? ++g : 0; }
struct Office { int issued = 0; __attribute__((noinline)) int next() { return issued < 3 ? ++issued : 0; } };
__attribute__((noinline)) int nextStatic() { return S::next(); }
template <class F> double timeit(F f) { double best = 1e18; for (int r = 0; r < 5; ++r) { auto t0 = std::chrono::steady_clock::now(); f(); auto t1 = std::chrono::steady_clock::now(); double ms = std::chrono::duration<double, std::milli>(t1 - t0).count(); if (ms < best) best = ms; } return best; }
int main()
{
    const int N = 100000000; volatile long long sink = 0;
    std::cout << "static member   : " << timeit([&]{ long long s=0; for (int i=0;i<N;++i) s+=nextStatic(); sink=s; }) << " ms for 100M requests\n";
    std::cout << "global variable : " << timeit([&]{ long long s=0; for (int i=0;i<N;++i) s+=nextGlobal(); sink=s; }) << " ms\n";
    Office o; std::cout << "Office object   : " << timeit([&]{ long long s=0; for (int i=0;i<N;++i) s+=o.next(); sink=s; }) << " ms\n";
    std::cout << "sizeof(counter): static 0 extra bytes per object; one int (" << sizeof(int) << ") per Office\n";
}
