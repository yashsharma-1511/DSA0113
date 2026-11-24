#pragma once
// REDESIGN: same interface, circular buffer with a start index (no shifting)
#include <iostream>
#ifndef HISTORY
#define HISTORY 10
#endif
namespace v2 {
struct Transaction { int no; char type; long long amount; };
class SavingsAccount {
    static const int CAPACITY = HISTORY;
    int accNo; long long balance; Transaction txn[CAPACITY]; int start; int count; int totalDone;   // + 'start'
    void record(char type, long long amount)
    {
        int pos = start + count; if (pos >= CAPACITY) pos -= CAPACITY;       // index of the next free slot
        if (count == CAPACITY) { txn[start] = {++totalDone, type, amount}; if (++start == CAPACITY) start = 0; }   // overwrite oldest: O(1)
        else { txn[pos] = {++totalDone, type, amount}; ++count; }
    }
public:
    SavingsAccount(int no, long long opening) : accNo(no), balance(opening), start(0), count(0), totalDone(0) {}
    long long getBalance() const { return balance; }
    void deposit(long long a) { if (a <= 0) return; balance += a; record('D', a); }
    bool withdraw(long long a) { if (a <= 0 || a > balance) return false; balance -= a; record('W', a); return true; }
    long long sumLast() const { long long s = 0; for (int i = 0; i < count; ++i) { int p = start + i; if (p >= CAPACITY) p -= CAPACITY; s += txn[p].amount; } return s; }
    void miniStatement(std::ostream& os) const
    {
        os << "Account " << accNo << " - last " << count << " transactions\n";
        for (int i = 0; i < count; ++i) { int p = start + i; if (p >= CAPACITY) p -= CAPACITY; os << "  #" << txn[p].no << ' ' << txn[p].type << ' ' << txn[p].amount << '\n'; }
        os << "  Balance: " << balance << '\n';
    }
};
}
