#include <iomanip>
#include <iostream>
#include <limits>
#include "metro.h"

// reads one journey "zone hour minute"; returns 1 = ok, 0 = rejected line, -1 = end of input
int readJourney(int& zone, int& hour, int& minute)
{
    std::cin >> zone >> hour >> minute;
    if (std::cin.eof() && std::cin.fail()) return -1;
    if (std::cin.fail()) {                       // letters, or a number that does not fit an int
        std::cin.clear();                        // 1. clear the error state
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');   // 2. discard the bad line
        return 0;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return 1;
}
void money(long long paise)                      // prints Rs 36.00 with <iomanip>
{
    std::cout << std::setw(4) << paise / 100 << '.' << std::setw(2) << std::setfill('0') << paise % 100 << std::setfill(' ');
}
int main()
{
    long long opening = 0;
    if (!(std::cin >> opening) || opening < 0) { std::cout << "Invalid opening balance\n"; return 1; }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    SmartCard card(opening * 100);
    std::cout << "No  Zone  Time   Fare(Rs)  Balance(Rs)  Result\n";
    int zone, hour, minute, no = 0, st;
    while ((st = readJourney(zone, hour, minute)) != -1) {
        ++no;
        if (st == 0)                     { std::cout << std::setw(2) << no << "  rejected: not a number\n"; continue; }
        if (zone < 0 || zone > 2 || hour < 0 || hour > 23 || minute < 0 || minute > 59)
                                         { std::cout << std::setw(2) << no << "  rejected: value out of range\n"; continue; }
        long long f = FareTable::fare(static_cast<Zone>(zone), hour * 60 + minute, Category::Regular);
        bool ok = card.deduct(f);
        std::cout << std::setw(2) << no << "  " << std::setw(4) << zone << "  " << std::setw(2) << std::setfill('0') << hour << ':'
                  << std::setw(2) << minute << std::setfill(' ') << ' ';
        money(f); std::cout << ' '; money(card.balance()); std::cout << "  " << (ok ? "OPEN" : "DENIED") << '\n';
    }
}
