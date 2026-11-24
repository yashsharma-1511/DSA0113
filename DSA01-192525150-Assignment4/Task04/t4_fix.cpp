#include <iostream>
#include <memory>
#include <vector>
class Account {
public:
    virtual ~Account() { std::cout << "~Account\n"; }              // virtual destructor
    virtual double rate() const { return 3.5; }                    // virtual: chosen by the real object
    Account() = default;
    Account(const Account&) = delete;                              // copying a base object would slice
    Account& operator=(const Account&) = delete;
};
class SeniorAccount : public Account {
    double* history = new double[100];
public:
    ~SeniorAccount() override { delete[] history; std::cout << "~SeniorAccount (history released)\n"; }
    double rate() const override { return 4.0; }
};
int main()
{
    Account* accounts[2] = { new Account, new SeniorAccount };
    for (Account* a : accounts) std::cout << a->rate() << "\n";
    for (Account* a : accounts) delete a;                          // now runs ~SeniorAccount, then ~Account
    std::cout << "--- no slicing ---\n";
    SeniorAccount s;
    Account& ref = s;                                              // a reference, not a copy
    std::cout << "ref.rate() = " << ref.rate() << "\n";
    std::vector<std::unique_ptr<Account>> v;                       // storing many accounts
    v.push_back(std::make_unique<SeniorAccount>());
    std::cout << "vector element rate = " << v[0]->rate() << "\n";
}
