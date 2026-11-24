// Same deterministic sequence of operations on either version; output is diffed
#include <iostream>
#ifdef USE_V2
#include "account_v2.h"
using namespace v2;
#else
#include "account_v1.h"
using namespace v1;
#endif
int main()
{
    SavingsAccount a(1001, 5000);
    unsigned long long s = 12345; long long accepted = 0, rejected = 0;
    for (int i = 0; i < 1000003; ++i) {
        s = s * 6364136223846793005ULL + 1442695040888963407ULL;
        long long amt = 1 + (s >> 33) % 900;
        if ((s >> 20) & 1) a.deposit(amt); else (a.withdraw(amt * 3) ? ++accepted : ++rejected);
    }
    std::cout << "withdrawals accepted " << accepted << ", rejected " << rejected << '\n';
    a.miniStatement(std::cout);
    SavingsAccount b(2002, 100);          // fewer transactions than the capacity
    b.deposit(5); b.withdraw(3); b.withdraw(500);
    b.miniStatement(std::cout);
}
