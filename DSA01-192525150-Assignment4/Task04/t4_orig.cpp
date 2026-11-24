#include <iostream>
#include <vector>
class Account { public: double rate() const { return 3.5; } ~Account() { std::cout << "~Account\n"; } };
class SeniorAccount : public Account {
 double* history = new double[100];
public: double rate() const { return 4.0; } ~SeniorAccount() { delete[] history; std::cout << "~SeniorAccount (history released)\n"; } };
int main()
{
 Account* accounts[2] = { new Account, new SeniorAccount };
 for (Account* a : accounts) std::cout << a->rate() << "\n";
 for (Account* a : accounts) delete a;
 std::cout << "--- slicing ---\n";
 Account copy = SeniorAccount();                // stored in a vector<Account> elsewhere
 std::cout << "copy.rate() = " << copy.rate() << " sizeof(copy) = " << sizeof(copy) << " sizeof(SeniorAccount) = " << sizeof(SeniorAccount) << "\n";
 std::vector<Account> v; v.push_back(SeniorAccount());
 std::cout << "vector element rate = " << v[0].rate() << "\n";
}
