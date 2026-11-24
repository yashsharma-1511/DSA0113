#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "metro.h"
// getline + stoi + try/catch ; every token must be fully consumed
bool toInt(const std::string& s, int& v)
{
    try { std::size_t pos = 0; v = std::stoi(s, &pos); return pos == s.size(); }
    catch (const std::exception&) { return false; }        // invalid_argument or out_of_range
}
int main()
{
    std::string line;
    std::getline(std::cin, line);
    SmartCard card(std::stoll(line) * 100);
    int no = 0;
    while (std::getline(std::cin, line)) {
        ++no;
        std::istringstream in(line); std::string a, b, c, extra; int zone, hour, minute;
        if (!(in >> a >> b >> c) || (in >> extra) || !toInt(a, zone) || !toInt(b, hour) || !toInt(c, minute)) { std::cout << std::setw(2) << no << "  rejected: not a number\n"; continue; }
        if (zone < 0 || zone > 2 || hour < 0 || hour > 23 || minute < 0 || minute > 59) { std::cout << std::setw(2) << no << "  rejected: value out of range\n"; continue; }
        long long f = FareTable::fare(static_cast<Zone>(zone), hour * 60 + minute, Category::Regular);
        bool ok = card.deduct(f);
        std::cout << std::setw(2) << no << "  fare " << f << " balance " << card.balance() << (ok ? " OPEN" : " DENIED") << '\n';
    }
}
