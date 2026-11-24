#include <iostream>
enum class Zone { Inner, Middle, Outer, Airport };      // fourth zone added to the enum only
constexpr long long BASE_PAISE[] = {1000, 2000, 3000};
long long fare(Zone z, bool peak)
{
 long long base = 0;
 switch (z) {
 case Zone::Inner:  base = BASE_PAISE[0]; break;
 case Zone::Middle: base = BASE_PAISE[1]; break;
 case Zone::Outer:  base = BASE_PAISE[2]; break;
 }
 if (peak) base += base * 20 / 100;
 return base;
}
int main()
{
 std::cout << "Airport weekday off-peak: " << fare(Zone::Airport, false) << " paise (should be 6000)\n";
 std::cout << "Airport peak            : " << fare(Zone::Airport, true) << " paise (should be 7200)\n";
 std::cout << "Outer Saturday 9 AM     : " << fare(Zone::Outer, true) << " paise (should be 2700: 10% off, no surcharge)\n";
}
