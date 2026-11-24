#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include "metro.h"
int main()
{
    long long sizes[] = {20000, 500000, 3000000};
    std::cout << std::fixed;
    for (long long n : sizes) {
        std::mt19937 rng(12345);
        long long exact = 0; double rupees = 0.0; double paiseAsDouble = 0.0; float f32 = 0.0f;
        std::uint32_t wrap = 0; long long firstOverflow = -1; long long runningForInt = 0;
        for (long long i = 0; i < n; ++i) {
            Zone z = static_cast<Zone>(rng() % 3);
            int minute = static_cast<int>(rng() % 1440);
            Category c = static_cast<Category>(rng() % 3);
            long long f = FareTable::fare(z, minute, c);
            exact += f;
            // double-rupee model of the same rules
            double r = static_cast<double>(BASE_PAISE[static_cast<int>(z)]) / 100.0;
            if (c != Category::Regular) r *= 0.5;
            if (FareTable::isPeak(minute)) r *= 1.2;
            rupees += r;
            paiseAsDouble += static_cast<double>(f);
            f32 += static_cast<float>(f) / 100.0f;
            wrap += static_cast<std::uint32_t>(f);
            runningForInt += f;
            if (firstOverflow < 0 && runningForInt > 2147483647LL) firstOverflow = i + 1;
        }
        std::cout << "n=" << n << "\n  long long paise : " << exact
                  << "\n  double paise    : " << std::setprecision(0) << paiseAsDouble << "  diff " << paiseAsDouble - static_cast<double>(exact)
                  << "\n  double rupees   : " << std::setprecision(6) << rupees << "  diff in paise " << std::setprecision(6) << rupees * 100.0 - static_cast<double>(exact)
                  << "\n  float rupees    : " << std::setprecision(2) << f32 << "  diff in paise " << (static_cast<double>(f32) * 100.0 - static_cast<double>(exact))
                  << "\n  int (wrapped)   : " << static_cast<std::int32_t>(wrap)
                  << "\n  first int overflow at journey: " << firstOverflow << "\n";
    }
    // time per addition
    const long long N = 200000000;
    volatile int vi; volatile long long vl; volatile double vd;
    auto t0 = std::chrono::steady_clock::now();
    std::uint32_t a = 0; for (long long i = 0; i < N; ++i) a += static_cast<std::uint32_t>(i % 5); vi = static_cast<int>(a);
    auto t1 = std::chrono::steady_clock::now();
    long long b = 0; for (long long i = 0; i < N; ++i) b += i % 5; vl = b;
    auto t2 = std::chrono::steady_clock::now();
    double c = 0; for (long long i = 0; i < N; ++i) c += static_cast<double>(i % 5); vd = c;
    auto t3 = std::chrono::steady_clock::now();
    std::cout << std::setprecision(0) << "200M additions: int " << std::chrono::duration<double, std::milli>(t1 - t0).count() << " ms, long long "
              << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms, double " << std::chrono::duration<double, std::milli>(t3 - t2).count() << " ms\n";
    (void)vi; (void)vl; (void)vd;
}
