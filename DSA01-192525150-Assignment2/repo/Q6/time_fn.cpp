#include "time.h"
#if VARIANT == 0
int minutesBetween(const Time& a, const Time& b) { int ta = a.h * 60 + a.m, tb = b.h * 60 + b.m; return ta > tb ? ta - tb : tb - ta; }
bool earlier(const Time& a, const Time& b) { return a.h * 60 + a.m < b.h * 60 + b.m; }
#endif
