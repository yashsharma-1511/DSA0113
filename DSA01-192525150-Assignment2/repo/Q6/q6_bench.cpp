#include "time.h"
#include <chrono>
#include <iostream>
#include <vector>
#if VARIANT == 2
#define MB(a, b) (a).minutesTo(b)
#define EA(a, b) (a).isEarlier(b)
#else
#define MB(a, b) minutesBetween(a, b)
#define EA(a, b) earlier(a, b)
#endif
int main()
{
    std::vector<Time> ts; for (int i = 0; i < 1024; ++i) ts.emplace_back((i * 7) % 24, (i * 13) % 60);
    const int N = 10000000; double best = 1e18; volatile long long sink = 0;
    for (int r = 0; r < 7; ++r) {
        long long s = 0; auto t0 = std::chrono::steady_clock::now();
        for (int i = 0; i < N; ++i) { const Time& a = ts[i & 1023]; const Time& b = ts[(i * 7 + 3) & 1023]; s += MB(a, b) + EA(a, b); }
        auto t1 = std::chrono::steady_clock::now(); sink = s;
        double ms = std::chrono::duration<double, std::milli>(t1 - t0).count(); if (ms < best) best = ms;
    }
    std::cout << best << " ms for 10,000,000 iterations (each makes one minutesBetween and one earlier call), checksum " << sink << '\n';
}
