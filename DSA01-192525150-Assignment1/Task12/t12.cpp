#include <iostream>
#include <iomanip>
enum class Zone { Inner, Middle, Outer, Airport, Count };
enum class Category { Regular, Student, Senior };
constexpr int ZONE_COUNT = static_cast<int>(Zone::Count);
constexpr long long BASE_PAISE[ZONE_COUNT] = {1000, 2000, 3000, 6000};   // Airport Rs 60
constexpr int PEAK_SURCHARGE_PERCENT = 20, CONCESSION_PERCENT = 50, WEEKEND_DISCOUNT_PERCENT = 10;
constexpr long long MIN_BALANCE_PAISE = 5000;
constexpr int PEAK_AM_START = 480, PEAK_AM_END = 600, PEAK_PM_START = 1020, PEAK_PM_END = 1200;
struct When { int day; int minute; int month; };           // day 0=Mon .. 6=Sun, minute of day, month = yyyymm
inline bool isWeekend(int day) { return day == 5 || day == 6; }
inline bool isPeak(int m) { return (m >= PEAK_AM_START && m < PEAK_AM_END) || (m >= PEAK_PM_START && m < PEAK_PM_END); }
inline long long pct(long long v, int p) { return (v * p + 50) / 100; }
class Pass {                                               // unlimited journeys in two zones for one month
    Zone a_, b_; int month_;
public:
    Pass(Zone a, Zone b, int month) : a_(a), b_(b), month_(month) {}
    bool covers(Zone z, int month) const { return month == month_ && (z == a_ || z == b_); }
};
class FareTable {
public:
    static long long fare(Zone z, const When& w, Category c) {
        long long f = BASE_PAISE[static_cast<int>(z)];
        if (c != Category::Regular) f -= pct(f, CONCESSION_PERCENT);
        if (isWeekend(w.day)) f -= pct(f, WEEKEND_DISCOUNT_PERCENT);       // never combined with peak
        else if (isPeak(w.minute)) f += pct(f, PEAK_SURCHARGE_PERCENT);
        return f;
    }
};
class SmartCard {
    long long balance_;
public:
    explicit SmartCard(long long b) : balance_(b) {}
    long long balance() const { return balance_; }
    bool canTravel(long long f) const { return balance_ - f >= MIN_BALANCE_PAISE; }
    bool deduct(long long f) { if (!canTravel(f)) return false; balance_ -= f; return true; }
    // returns paid fare, or -1 if denied; a pass journey is free and leaves the balance alone
    long long travel(Zone z, const When& w, Category c, const Pass* p) {
        if (p && p->covers(z, w.month)) return 0;
        long long f = FareTable::fare(z, w, c);
        return deduct(f) ? f : -1;
    }
};
int fails = 0, total = 0;
void check(const char* what, long long exp, long long act)
{ ++total; bool ok = exp == act; if (!ok) ++fails; std::cout << (ok ? "Pass: " : "FAIL: ") << std::left << std::setw(40) << what << std::right << " expected " << exp << " actual " << act << '\n'; }
int main()
{
    const Category R = Category::Regular;
    check("Airport weekday off-peak", 6000, FareTable::fare(Zone::Airport, {1, 720, 202611}, R));
    check("Airport weekday peak", 7200, FareTable::fare(Zone::Airport, {1, 540, 202611}, R));
    check("Outer Saturday 09:00 (no surcharge)", 2700, FareTable::fare(Zone::Outer, {5, 540, 202611}, R));
    check("Airport Sunday 18:00 regular", 5400, FareTable::fare(Zone::Airport, {6, 1080, 202611}, R));
    check("Airport Saturday 09:00 student", 2700, FareTable::fare(Zone::Airport, {5, 540, 202611}, Category::Student));
    check("Middle weekday off-peak (old rule)", 2000, FareTable::fare(Zone::Middle, {2, 720, 202611}, R));
    check("Inner 08:00 weekday peak (old rule)", 1200, FareTable::fare(Zone::Inner, {0, 480, 202611}, R));
    check("Inner 10:00 weekday (old rule)", 1000, FareTable::fare(Zone::Inner, {0, 600, 202611}, R));
    Pass pass(Zone::Inner, Zone::Middle, 202611);
    SmartCard c(10000);
    check("Inner journey with pass", 0, c.travel(Zone::Inner, {1, 540, 202611}, R, &pass));
    check("Middle journey with pass", 0, c.travel(Zone::Middle, {1, 540, 202611}, R, &pass));
    check("balance unchanged by pass journeys", 10000, c.balance());
    check("Outer not covered (peak 3600)", 3600, c.travel(Zone::Outer, {1, 540, 202611}, R, &pass));
    check("balance after Outer", 6400, c.balance());
    SmartCard c2(10000);
    check("next month not covered (fare 2000)", 2000, c2.travel(Zone::Middle, {1, 720, 202612}, R, &pass));
    check("Airport peak denied (-1)", -1, c.travel(Zone::Airport, {1, 540, 202611}, R, &pass));
    std::cout << (fails == 0 ? "ALL PASSED" : "SOME FAILED") << " (" << total - fails << "/" << total << ")\n";
}
