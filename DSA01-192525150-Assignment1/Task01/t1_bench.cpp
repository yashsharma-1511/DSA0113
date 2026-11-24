#include <chrono>
#include <iostream>
#include "metro.h"
long long gBalance = 100000000000LL; const long long GBASE[] = {1000, 2000, 3000};
long long fareOf(int zone, int minute) { long long f = GBASE[zone]; bool peak = (minute >= 480 && minute < 600) || (minute >= 1020 && minute < 1200); if (peak) f += (f * 20 + 50) / 100; return f; }
int main()
{
    const long long N = 50000000; long long cnt1 = 0, cnt2 = 0;
    auto t0 = std::chrono::steady_clock::now();
    for (long long i = 0; i < N; ++i) { long long f = fareOf(static_cast<int>(i % 3), static_cast<int>(i % 1440)); if (gBalance - f >= 5000) { gBalance -= f; ++cnt1; } }
    auto t1 = std::chrono::steady_clock::now();
    SmartCard card(100000000000LL);
    for (long long i = 0; i < N; ++i) { long long f = FareTable::fare(static_cast<Zone>(i % 3), static_cast<int>(i % 1440), Category::Regular); if (card.deduct(f)) ++cnt2; }
    auto t2 = std::chrono::steady_clock::now();
    std::cout << "procedural " << std::chrono::duration<double, std::milli>(t1 - t0).count() << " ms, object-oriented " << std::chrono::duration<double, std::milli>(t2 - t1).count()
              << " ms, accepted " << cnt1 << " " << cnt2 << ", sizeof(long long)=" << sizeof(long long) << ", sizeof(SmartCard)=" << sizeof(SmartCard) << '\n';
}
