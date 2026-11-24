// Several concessions on one fee: return-the-new-fee design, plus order dependence of integer rounding
#include <iostream>
long long applyConcession(long long fee, int percent) { return fee - fee * percent / 100; }
void applyInPlace(long long& fee, int percent) { fee = applyConcession(fee, percent); }
int main()
{
    long long original = 45000;
    long long merit = applyConcession(original, 10);
    long long both = applyConcession(merit, 15);
    long long all3 = applyConcession(both, 25);
    std::cout << "original " << original << " -> merit " << merit << " -> +sports " << both
              << " -> +single parent " << all3 << " (original still " << original << ")\n";
    long long f = 45000; applyInPlace(f, 10); applyInPlace(f, 15); applyInPlace(f, 25);
    std::cout << "in place, same three: " << f << '\n';
    int found = 0;
    for (long long fee = 1; fee <= 100000 && found < 3; ++fee) {
        long long x = applyConcession(applyConcession(fee, 10), 25);
        long long y = applyConcession(applyConcession(fee, 25), 10);
        if (x != y) {
            std::cout << "order matters: fee " << fee << ": 10% then 25% = " << x << ", 25% then 10% = " << y << '\n';
            ++found;
        }
    }
    long long diff = 0;
    for (long long fee = 1; fee <= 100000; ++fee)
        diff += applyConcession(applyConcession(fee, 10), 25) != applyConcession(applyConcession(fee, 25), 10);
    std::cout << "fees 1..100000 where order changes the result: " << diff << '\n';
}
