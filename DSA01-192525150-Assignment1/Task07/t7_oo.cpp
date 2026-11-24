#include <iostream>
#include "metro.h"
int main()
{
 SmartCard card(10000);
 Zone zones[] = {Zone::Inner, Zone::Outer, Zone::Middle, Zone::Inner, Zone::Outer};
 int minutes[] = {12 * 60, 9 * 60, 14 * 60, 18 * 60, 13 * 60};
 for (int i = 0; i < 5; ++i) {
  long long f = FareTable::fare(zones[i], minutes[i], Category::Regular);
  if (card.deduct(f)) std::cout << "journey " << i + 1 << ": fare " << f << " balance " << card.balance() << '\n';
  else std::cout << "journey " << i + 1 << ": denied (fare " << f << ") balance " << card.balance() << '\n';
 }
 // card.balance_ -= 9000;      // error: 'balance_' is private within this context
 std::cout << "deduct(9000) accepted? " << std::boolalpha << card.deduct(9000) << ", balance " << card.balance() << '\n';
}
