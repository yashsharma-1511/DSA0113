#include <chrono>
#include <iostream>
#include <memory>
#include <vector>
struct AccountNV { double rate() const { return 3.5; } ~AccountNV() { } };           // no virtual anything
struct SeniorNV : AccountNV { double* history = new double[100]; ~SeniorNV() { delete[] history; } double rate() const { return 4.0; } };
struct Account { virtual ~Account() = default; virtual double rate() const { return 3.5; } };
struct Senior : Account { double* history = new double[100]; ~Senior() override { delete[] history; } double rate() const override { return 4.0; } };
int main(int argc, char**)
{
    if (argc > 1) {                                                         // defects that survive unique_ptr
        std::vector<std::unique_ptr<AccountNV>> v;
        v.emplace_back(std::unique_ptr<AccountNV>(new SeniorNV));
        std::cout << "rate through unique_ptr<AccountNV> = " << v[0]->rate() << '\n';
        return 0;
    }
    const int N = 1000000; volatile double sink = 0; double s = 0;
    auto t0 = std::chrono::steady_clock::now();
    { std::vector<Account*> raw; for (int i = 0; i < N; ++i) raw.push_back(new Senior);
      for (Account* a : raw) s += a->rate();
      for (Account* a : raw) delete a; }
    auto t1 = std::chrono::steady_clock::now();
    { std::vector<std::unique_ptr<Account>> sm; for (int i = 0; i < N; ++i) sm.push_back(std::make_unique<Senior>());
      for (auto& a : sm) s += a->rate(); }
    auto t2 = std::chrono::steady_clock::now();
    sink = s; (void)sink;
    std::cout << "1 million accounts, create + process + release: raw " << std::chrono::duration<double, std::milli>(t1 - t0).count() << " ms, unique_ptr " << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms; sizeof(unique_ptr<Account>) = " << sizeof(std::unique_ptr<Account>) << " = sizeof(Account*) = " << sizeof(Account*) << '\n';
}
