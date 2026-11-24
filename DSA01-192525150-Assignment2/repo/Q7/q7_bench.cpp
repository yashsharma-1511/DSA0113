#include "account_v1.h"
#include "account_v2.h"
#include <chrono>
#include <cstdlib>
#include <iostream>
template <class A> double run(long long N, long long& chk)
{
    double best = 1e18;
    for (int r = 0; r < 3; ++r) {
        A acc(1, 1000000000LL); auto t0 = std::chrono::steady_clock::now();
        for (long long i = 0; i < N; ++i) { if (i & 1) acc.withdraw(i % 1000 + 1); else acc.deposit(i % 1000 + 1); asm volatile("" : : "r"(&acc) : "memory"); }
        auto t1 = std::chrono::steady_clock::now(); chk = acc.getBalance() + acc.sumLast();
        double ns = std::chrono::duration<double, std::nano>(t1 - t0).count() / N; if (ns < best) best = ns;
    }
    return best;
}
int main(int argc, char** argv)
{
    long long N = argc > 1 ? std::atoll(argv[1]) : 1000000, c1, c2;
    double a = run<v1::SavingsAccount>(N, c1), b = run<v2::SavingsAccount>(N, c2);
    std::cout << "K=" << HISTORY << " N=" << N << " original " << a << " ns/add, redesign " << b << " ns/add, speed-up " << a / b << "x, balances equal " << std::boolalpha << (c1 == c2)
              << ", sizeof " << sizeof(v1::SavingsAccount) << " -> " << sizeof(v2::SavingsAccount) << '\n';
}
