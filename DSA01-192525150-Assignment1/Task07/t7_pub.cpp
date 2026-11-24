#include <iostream>
struct OpenCard { long long balance; };
int main()
{
 OpenCard c{10000};
 c.balance -= 9000;                    // compiles; Rs 50 rule broken
 c.balance -= 3000;                    // negative balance
 std::cout << "public balance now " << c.balance << " paise\n";
}
