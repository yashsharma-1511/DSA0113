#pragma once
// VARIANT 0: friend functions defined in time_fn.cpp (not inline)
// VARIANT 1: friend functions defined inline in this header
// VARIANT 2: member functions minutesTo() / isEarlier()
// VARIANT 3: public getters + free functions
class Time {
    int h, m;
public:
    Time(int hh, int mm) : h(hh), m(mm) {}
#if VARIANT <= 1
    friend int minutesBetween(const Time& a, const Time& b);
    friend bool earlier(const Time& a, const Time& b);
#elif VARIANT == 2
    int minutesTo(const Time& o) const { int ta = h * 60 + m, tb = o.h * 60 + o.m; return ta > tb ? ta - tb : tb - ta; }
    bool isEarlier(const Time& o) const { return h * 60 + m < o.h * 60 + o.m; }
#else
    int hour() const { return h; }
    int minute() const { return m; }
#endif
};
#if VARIANT == 1
inline int minutesBetween(const Time& a, const Time& b) { int ta = a.h * 60 + a.m, tb = b.h * 60 + b.m; return ta > tb ? ta - tb : tb - ta; }
inline bool earlier(const Time& a, const Time& b) { return a.h * 60 + a.m < b.h * 60 + b.m; }
#elif VARIANT == 3
inline int minutesBetween(const Time& a, const Time& b) { int ta = a.hour() * 60 + a.minute(), tb = b.hour() * 60 + b.minute(); return ta > tb ? ta - tb : tb - ta; }
inline bool earlier(const Time& a, const Time& b) { return a.hour() * 60 + a.minute() < b.hour() * 60 + b.minute(); }
#endif
#if VARIANT == 0
int minutesBetween(const Time& a, const Time& b);
bool earlier(const Time& a, const Time& b);
#endif
