#include <iostream>
class SavingsAccount { long long balance = 100; long long txn[10] = {}; public: long long get() const { return balance; } };
int main() { SavingsAccount a; a.balance = 1000000; a.txn[10] = 5; std::cout << a.get(); }
