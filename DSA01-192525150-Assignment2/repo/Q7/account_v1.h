#pragma once
// ORIGINAL design (from Q4): fixed array, oldest entry discarded by shifting
#include <iostream>
#ifndef HISTORY
#define HISTORY 10
#endif
namespace v1 {
struct Transaction { int no; char type; long long amount; };
class SavingsAccount {
    static const int CAPACITY = HISTORY;
    int accNo; long long balance; Transaction txn[CAPACITY]; int count; int totalDone;
    void record(char type, long long amount)
    {
        if (count == CAPACITY) { for (int i = 0; i < CAPACITY - 1; ++i) txn[i] = txn[i + 1]; --count; }   // O(K)
        txn[count++] = {++totalDone, type, amount};
    }
public:
    SavingsAccount(int no, long long opening) : accNo(no), balance(opening), count(0), totalDone(0) {}
    long long getBalance() const { return balance; }
    void deposit(long long a) { if (a <= 0) return; balance += a; record('D', a); }
    bool withdraw(long long a) { if (a <= 0 || a > balance) return false; balance -= a; record('W', a); return true; }
    long long sumLast() const { long long s = 0; for (int i = 0; i < count; ++i) s += txn[i].amount; return s; }
    void miniStatement(std::ostream& os) const
    {
        os << "Account " << accNo << " - last " << count << " transactions\n";
        for (int i = 0; i < count; ++i) os << "  #" << txn[i].no << ' ' << txn[i].type << ' ' << txn[i].amount << '\n';
        os << "  Balance: " << balance << '\n';
    }
};
}
