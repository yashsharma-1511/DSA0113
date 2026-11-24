#include <iomanip>
#include <iostream>
enum class Zone { Inner, Middle, Outer };
constexpr int BASE_PAISE[] = {1000, 2000, 3000};
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;
constexpr int PEAK_AM_START = 8 * 60, PEAK_AM_END = 10 * 60;   // [08:00, 10:00)
constexpr int PEAK_PM_START = 17 * 60, PEAK_PM_END = 20 * 60;  // [17:00, 20:00)
bool isPeak(int minuteOfDay)
{
 return (minuteOfDay >= PEAK_AM_START && minuteOfDay < PEAK_AM_END) ||
        (minuteOfDay >= PEAK_PM_START && minuteOfDay < PEAK_PM_END);
}
long long fare(Zone z, bool peak)
{
 long long base = 0;
 switch (z) {
 case Zone::Inner:  base = BASE_PAISE[0]; break;
 case Zone::Middle: base = BASE_PAISE[1]; break;
 case Zone::Outer:  base = BASE_PAISE[2]; break;
 }
 if (peak) base += base * PEAK_SURCHARGE_PERCENT / 100;
 return base;
}
bool canTravel(long long balance, long long f) { return balance - f >= MIN_BALANCE_PAISE; }
void show(const char* label, int h, int m)
{
 int t = h * 60 + m;
 long long f = fare(Zone::Middle, isPeak(t));
 std::cout << label << " " << std::setw(2) << std::setfill('0') << h << ':' << std::setw(2) << m
           << std::setfill(' ') << " peak=" << std::boolalpha << isPeak(t) << " Middle fare=" << f
           << " paise, travel with balance 7000? " << canTravel(7000, f) << '\n';
}
int main()
{
 show("start of morning peak   ", 8, 0);
 show("last minute of morning  ", 9, 59);
 show("exactly 10 AM           ", 10, 0);
 show("start of evening peak   ", 17, 0);
 show("last minute of evening  ", 19, 59);
 show("exactly 8 PM            ", 20, 0);
}
