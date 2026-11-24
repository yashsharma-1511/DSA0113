// metro.h : the ONLY place where fares, limits and fare rules are written
#pragma once
enum class Zone { Inner, Middle, Outer };
enum class Category { Regular, Student, Senior };

constexpr long long BASE_PAISE[] = {1000, 2000, 3000};   // Rs 10, Rs 20, Rs 30
constexpr int  PEAK_SURCHARGE_PERCENT = 20;
constexpr int  CONCESSION_PERCENT     = 50;               // assumption A1
constexpr long long MIN_BALANCE_PAISE = 5000;             // Rs 50 must remain
constexpr int  PEAK_AM_START = 8 * 60,  PEAK_AM_END = 10 * 60;   // [08:00, 10:00)
constexpr int  PEAK_PM_START = 17 * 60, PEAK_PM_END = 20 * 60;   // [17:00, 20:00)

class FareTable {
public:
    static bool isPeak(int minuteOfDay) {
        return (minuteOfDay >= PEAK_AM_START && minuteOfDay < PEAK_AM_END) ||
               (minuteOfDay >= PEAK_PM_START && minuteOfDay < PEAK_PM_END);
    }
    static long long percentOf(long long v, int pct) { return (v * pct + 50) / 100; }  // half-up
    static long long fare(Zone z, int minuteOfDay, Category c) {
        long long f = BASE_PAISE[static_cast<int>(z)];
        if (c != Category::Regular) f -= percentOf(f, CONCESSION_PERCENT);
        if (isPeak(minuteOfDay))    f += percentOf(f, PEAK_SURCHARGE_PERCENT);
        return f;
    }
};

class SmartCard {
    long long balance_;                                   // paise, private
public:
    explicit SmartCard(long long openingPaise = 0) : balance_(openingPaise < 0 ? 0 : openingPaise) {}
    long long balance() const { return balance_; }
    bool canTravel(long long f) const { return f >= 0 && balance_ - f >= MIN_BALANCE_PAISE; }
    bool deduct(long long f) { if (!canTravel(f)) return false; balance_ -= f; return true; }
    bool recharge(long long a) { if (a <= 0) return false; balance_ += a; return true; }
};
