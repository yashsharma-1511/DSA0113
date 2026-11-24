#include <chrono>
#include <iostream>
#include "metro.h"
struct Case { const char* id; Zone z; int h, m; Category c; long long expected; };
const Case CASES[] = {
 {"N1", Zone::Inner, 12, 0, Category::Regular, 1000}, {"N2", Zone::Middle, 12, 0, Category::Regular, 2000},
 {"N3", Zone::Outer, 12, 0, Category::Regular, 3000}, {"N4", Zone::Inner, 9, 0, Category::Regular, 1200},
 {"N5", Zone::Middle, 18, 0, Category::Regular, 2400}, {"N6", Zone::Outer, 8, 30, Category::Regular, 3600},
 {"N7", Zone::Middle, 12, 0, Category::Student, 1000}, {"N8", Zone::Outer, 18, 0, Category::Senior, 1800},
 {"B1", Zone::Middle, 8, 0, Category::Regular, 2400}, {"B2", Zone::Middle, 7, 59, Category::Regular, 2000},
 {"B3", Zone::Middle, 9, 59, Category::Regular, 2400}, {"B4", Zone::Middle, 10, 0, Category::Regular, 2000},
 {"B5", Zone::Middle, 17, 0, Category::Regular, 2400}, {"B6", Zone::Middle, 16, 59, Category::Regular, 2000},
 {"B7", Zone::Middle, 19, 59, Category::Regular, 2400}, {"B8", Zone::Middle, 20, 0, Category::Regular, 2000},
 {"B11", Zone::Inner, 0, 0, Category::Regular, 1000},   // new case: one added line only
};
int main()
{
    int failed = 0, n = static_cast<int>(sizeof(CASES) / sizeof(CASES[0]));
    for (const Case& c : CASES) { long long a = FareTable::fare(c.z, c.h * 60 + c.m, c.c); if (a != c.expected) { ++failed; std::cout << c.id << " FAIL " << a << '\n'; } }
    std::cout << n - failed << " of " << n << " table cases passed\n";
    const int REP = 1000000; long long sink = 0;
    auto t0 = std::chrono::steady_clock::now();
    for (int r = 0; r < REP; ++r) for (const Case& c : CASES) sink += FareTable::fare(c.z, c.h * 60 + c.m, c.c);
    auto t1 = std::chrono::steady_clock::now();
    std::cout << REP * n << " case evaluations in " << std::chrono::duration<double, std::milli>(t1 - t0).count() << " ms (checksum " << sink << "), sizeof(CASES)=" << sizeof(CASES) << " bytes\n";
}
