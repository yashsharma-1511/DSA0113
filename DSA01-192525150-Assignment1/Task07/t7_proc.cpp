#include <iostream>
long long balance = 10000;                       // global: any function can change it
const long long BASE[] = {1000, 2000, 3000};
const long long MIN_BAL = 5000;
long long fareOf(int zone, bool peak) { long long f = BASE[zone]; if (peak) f += (f * 20 + 50) / 100; return f; }
int main()
{
 int zones[] = {0, 2, 1, 0, 2}; bool peaks[] = {false, true, false, true, false};
 for (int i = 0; i < 5; ++i) {
  long long f = fareOf(zones[i], peaks[i]);
  if (balance - f >= MIN_BAL) { balance -= f; std::cout << "journey " << i + 1 << ": fare " << f << " balance " << balance << '\n'; }
  else std::cout << "journey " << i + 1 << ": denied (fare " << f << ") balance " << balance << '\n';
 }
 balance -= 9000;                                // a careless line elsewhere: nothing stops it
 std::cout << "after careless balance -= 9000 : " << balance << '\n';
}
