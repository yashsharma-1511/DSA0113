#include <iostream>
class Time {
    int h, m;
public:
    Time(int hh, int mm) : h(hh), m(mm) {}
    friend int minutesBetween(const Time& a, const Time& b);
    friend bool earlier(const Time& a, const Time& b);
};
int minutesBetween(const Time& a, const Time& b) { int ta = a.h * 60 + a.m, tb = b.h * 60 + b.m; return ta > tb ? ta - tb : tb - ta; }
bool earlier(const Time& a, const Time& b) { return a.h * 60 + a.m < b.h * 60 + b.m; }
static int fails = 0;
void check(const char* name, long long expect, long long got) { bool ok = expect == got; if (!ok) ++fails; std::cout << (ok ? "PASS " : "FAIL ") << name << " expected " << expect << " got " << got << '\n'; }
int main()
{
    std::cout << std::boolalpha;
    check("09:45 -> 13:10 minutes", 205, minutesBetween(Time(9,45), Time(13,10)));
    check("13:10 -> 09:45 minutes (symmetric)", 205, minutesBetween(Time(13,10), Time(9,45)));
    check("same time twice, minutes", 0, minutesBetween(Time(10,30), Time(10,30)));
    check("same time twice, earlier(a,b)", 0, earlier(Time(10,30), Time(10,30)));
    check("11:50 -> 12:20 across noon", 30, minutesBetween(Time(11,50), Time(12,20)));
    check("earlier(11:50, 12:20)", 1, earlier(Time(11,50), Time(12,20)));
    check("earlier(12:20, 11:50)", 0, earlier(Time(12,20), Time(11,50)));
    check("11:59 -> 12:00", 1, minutesBetween(Time(11,59), Time(12,0)));
    check("earlier(12:00, 11:59)", 0, earlier(Time(12,0), Time(11,59)));
    check("00:00 -> 23:59", 1439, minutesBetween(Time(0,0), Time(23,59)));
    std::cout << (fails == 0 ? "ALL PASSED" : "SOME FAILED") << '\n';
}
