#include <iostream>
#include <string>
#include <iomanip>
#include "metro.h"
int failures = 0, total = 0;
void check(const char* id, const char* what, long long expected, long long actual)
{
    ++total; bool ok = expected == actual; if (!ok) ++failures;
    std::cout << std::left << std::setw(4) << id << std::setw(40) << what << std::right << std::setw(8) << expected << std::setw(8) << actual << "  " << (ok ? "PASS" : "FAIL") << '\n';
}
bool validZone(int z) { return z >= 0 && z <= 2; }
bool parseHour(const std::string& s, int& h)
{
    try { std::size_t p = 0; h = std::stoi(s, &p); return p == s.size() && h >= 0 && h <= 23; } catch (...) { return false; }
}
int main()
{
    auto F = [](Zone z, int h, int m, Category c) { return FareTable::fare(z, h * 60 + m, c); };
    std::cout << "ID  Case                                    Expect  Actual  Result\n";
    check("N1", "Inner 12:00 regular", 1000, F(Zone::Inner, 12, 0, Category::Regular));
    check("N2", "Middle 12:00 regular", 2000, F(Zone::Middle, 12, 0, Category::Regular));
    check("N3", "Outer 12:00 regular", 3000, F(Zone::Outer, 12, 0, Category::Regular));
    check("N4", "Inner 09:00 peak", 1200, F(Zone::Inner, 9, 0, Category::Regular));
    check("N5", "Middle 18:00 peak", 2400, F(Zone::Middle, 18, 0, Category::Regular));
    check("N6", "Outer 08:30 peak", 3600, F(Zone::Outer, 8, 30, Category::Regular));
    check("N7", "Student Middle 12:00", 1000, F(Zone::Middle, 12, 0, Category::Student));
    check("N8", "Senior Outer 18:00 peak", 1800, F(Zone::Outer, 18, 0, Category::Senior));
    check("B1", "Middle 08:00 (peak starts)", 2400, F(Zone::Middle, 8, 0, Category::Regular));
    check("B2", "Middle 07:59", 2000, F(Zone::Middle, 7, 59, Category::Regular));
    check("B3", "Middle 09:59 (last peak minute)", 2400, F(Zone::Middle, 9, 59, Category::Regular));
    check("B4", "Middle 10:00 (peak over)", 2000, F(Zone::Middle, 10, 0, Category::Regular));
    check("B5", "Middle 17:00 (peak starts)", 2400, F(Zone::Middle, 17, 0, Category::Regular));
    check("B6", "Middle 16:59", 2000, F(Zone::Middle, 16, 59, Category::Regular));
    check("B7", "Middle 19:59 (last peak minute)", 2400, F(Zone::Middle, 19, 59, Category::Regular));
    check("B8", "Middle 20:00 (peak over)", 2000, F(Zone::Middle, 20, 0, Category::Regular));
    check("B9", "balance 7000, fare 2000 (leaves 5000)", 1, SmartCard(7000).canTravel(2000));
    check("B10", "balance 6999, fare 2000 (leaves 4999)", 0, SmartCard(6999).canTravel(2000));
    check("I1", "zone 5 accepted? (0 = rejected)", 0, validZone(5));
    SmartCard c(10000);
    check("I2", "recharge(-500) accepted?", 0, c.recharge(-500));
    check("I3", "balance unchanged after bad recharge", 10000, c.balance());
    check("I4", "recharge(0) accepted?", 0, c.recharge(0));
    int h = 0;
    check("I5", "hour \"ab\" accepted?", 0, parseHour("ab", h));
    check("I6", "hour 24 accepted?", 0, parseHour("24", h));
    check("I7", "deduct(9000) from 10000 accepted?", 0, c.deduct(9000));
    check("I8", "balance unchanged after refused deduct", 10000, c.balance());
    SmartCard d(20000); long long sum = 0;
    long long f1 = F(Zone::Inner, 12, 0, Category::Regular);  d.deduct(f1); sum += f1;
    long long f2 = F(Zone::Middle, 9, 0, Category::Regular);  d.deduct(f2); sum += f2;
    long long f3 = F(Zone::Outer, 18, 0, Category::Student);  d.deduct(f3); sum += f3;
    long long f4 = F(Zone::Outer, 14, 0, Category::Senior);   d.deduct(f4); sum += f4;
    long long f5 = F(Zone::Inner, 19, 59, Category::Regular); d.deduct(f5); sum += f5;
    check("H1", "5-journey total fare (hand: 7900)", 7900, sum);
    check("H2", "balance after 5 journeys (hand: 12100)", 12100, d.balance());
    std::cout << total - failures << " of " << total << " passed\n";
}
