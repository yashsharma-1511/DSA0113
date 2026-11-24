// What is lost when h and m are public
#include <iostream>
struct Time { int h, m; };
int minutesBetween(const Time& a, const Time& b) { int ta = a.h * 60 + a.m, tb = b.h * 60 + b.m; return ta > tb ? ta - tb : tb - ta; }
int main()
{
    Time t{9, 45};
    t.m = 75;                    // compiles: nothing stops an impossible minute value
    t.h = -3;                    // compiles: negative hour
    Time u{9, 45};
    std::cout << "t = " << t.h << ':' << t.m << " (not a real time)\n";
    std::cout << "minutesBetween(t, u) = " << minutesBetween(t, u) << '\n';
    Time v{10, 15}, w{9, 75};    // 9:75 is really 10:15, but the fields differ
    std::cout << "same real time 10:15 vs 9:75 -> minutesBetween = " << minutesBetween(v, w) << ", field compare v.h==w.h ? " << std::boolalpha << (v.h == w.h) << '\n';
}
