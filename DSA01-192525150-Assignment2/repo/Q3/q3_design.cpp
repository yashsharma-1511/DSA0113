#include <iomanip>
#include <iostream>
struct Parcel { double kg; bool express; bool insured; };
constexpr double RATE_PER_KG = 40.0;       // Rs per kg
constexpr double EXPRESS_EXTRA = 0.5;      // +50 % of the base charge
constexpr double INSURANCE_FLAT = 25.0;    // Rs
static double price(double kg, bool express, bool insured)
{
    double base = RATE_PER_KG * kg;
    if (express) base += base * EXPRESS_EXTRA;
    if (insured) base += INSURANCE_FLAT;
    return base;
}
double charge(int grams,  bool express = false, bool insured = false) { std::cout << "[grams  ] "; return price(grams / 1000.0, express, insured); }
double charge(double kg,  bool express = false, bool insured = false) { std::cout << "[kg     ] "; return price(kg, express, insured); }
double charge(const Parcel& p)                                         { std::cout << "[parcel ] "; return price(p.kg, p.express, p.insured); }
int main()
{
    Parcel parcel{3.0, true, true};
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "charge(2)                    = " << charge(2) << '\n';
    std::cout << "charge(2.0f)                 = " << charge(2.0f) << '\n';
    std::cout << "charge(2.5)                  = " << charge(2.5) << '\n';
    std::cout << "charge(1500, true)           = " << charge(1500, true) << '\n';
    std::cout << "charge(1.5, true, true)      = " << charge(1.5, true, true) << '\n';
    std::cout << "charge(parcel)               = " << charge(parcel) << '\n';
}
