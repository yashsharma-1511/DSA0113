#include <iostream>
#ifndef HISTORY
#define HISTORY 10
#endif

struct Transaction { int no; char type; long long amount; };   // 'D' deposit, 'W' withdrawal

class SavingsAccount {
    static const int CAPACITY = HISTORY;   // the ONLY place the history size is written
    int accNo;
    long long balance;
    Transaction txn[CAPACITY];             // array inside the class, private
    int count;                             // how many entries are stored (<= CAPACITY)
    int totalDone;                         // transactions recorded so far (for numbering)
    void record(char type, long long amount);   // defined outside
public:
    SavingsAccount(int no, long long opening) : accNo(no), balance(opening), count(0), totalDone(0) {}
    long long getBalance() const { return balance; }        // defined inside the class
    void deposit(long long amount)                          // defined inside the class
    {
        if (amount <= 0) return;
        balance += amount;
        record('D', amount);
    }
    bool withdraw(long long amount);                        // defined outside
    void miniStatement() const;                             // defined outside
};

void SavingsAccount::record(char type, long long amount)
{
    if (count == CAPACITY) {                       // 11th transaction: discard the oldest by shifting
        for (int i = 0; i < CAPACITY - 1; ++i) txn[i] = txn[i + 1];
        --count;
    }
    txn[count++] = {++totalDone, type, amount};
}
bool SavingsAccount::withdraw(long long amount)
{
    if (amount <= 0 || amount > balance) return false;     // balance can never go negative
    balance -= amount;
    record('W', amount);
    return true;
}
void SavingsAccount::miniStatement() const
{
    std::cout << "Account " << accNo << " - last " << count << " transactions\n";
    for (int i = 0; i < count; ++i)
        std::cout << "  #" << txn[i].no << ' ' << (txn[i].type == 'D' ? "Deposit   " : "Withdrawal") << ' ' << txn[i].amount << '\n';
    std::cout << "  Balance: " << balance << '\n';
}
int main()
{
    SavingsAccount a(1001, 5000);
    long long ops[] = {100, 200, -50, 300, -80, 400, -120, 500, -60, 600};   // 10 transactions (negative = withdrawal)
    for (long long o : ops) { if (o > 0) a.deposit(o); else a.withdraw(-o); }
    std::cout << "after 10 transactions:\n"; a.miniStatement();
    a.withdraw(70);
    std::cout << "after the 11th transaction:\n"; a.miniStatement();
    std::cout << "overdraft attempt accepted? " << std::boolalpha << a.withdraw(1000000) << '\n';
    a.deposit(700);
    std::cout << "after the 12th transaction:\n"; a.miniStatement();
    std::cout << "sizeof(Transaction)=" << sizeof(Transaction) << " sizeof(SavingsAccount)=" << sizeof(SavingsAccount) << '\n';
}
