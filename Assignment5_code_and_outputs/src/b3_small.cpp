// B3.1: the same 10 bills totalled by (a) virtual functions and (b) one struct + switch
#include <iostream>
#include <memory>
#include <vector>
using Paise = long long;
struct Bill { virtual ~Bill() = default; virtual Paise total() const = 0; };
struct OP : Bill { Paise c, t; OP(Paise c, Paise t) : c(c), t(t) {} Paise total() const override { return c + t; } };
struct IP : Bill { int d; Paise r, p; IP(int d, Paise r, Paise p) : d(d), r(r), p(p) {} Paise total() const override { return d * r + p; } };
struct DC : Bill { Paise pk; explicit DC(Paise p) : pk(p) {} Paise total() const override { return pk; } };
struct ER : Bill { Paise b; int h; Paise r; ER(Paise b, int h, Paise r) : b(b), h(h), r(r) {} Paise total() const override { return b + h * r; } };
enum Kind { KOP, KIP, KDC, KER };
struct Rec { Kind k; Paise a, b, c; };   // OP: a=cons,b=tests | IP: a=days,b=rate,c=extras | DC: a=package | ER: a=base,b=hours,c=rate
Paise totalSwitch(const Rec& r) {
    switch (r.k) { case KOP: return r.a + r.b; case KIP: return r.a * r.b + r.c; case KDC: return r.a; case KER: return r.a + r.b * r.c; }
    return 0;
}
int main() {
    std::vector<std::unique_ptr<Bill>> v; std::vector<Rec> r; Paise hand[10] = {170000, 4900000, 3500000, 300000, 90000, 2450000, 1200000, 450000, 7600000, 125000};
    v.push_back(std::make_unique<OP>(50000, 120000));            r.push_back({KOP, 50000, 120000, 0});            // 1,700.00
    v.push_back(std::make_unique<IP>(3, 250000, 4150000));       r.push_back({KIP, 3, 250000, 4150000});          // 7,500+41,500 = 49,000.00
    v.push_back(std::make_unique<DC>(3500000));                  r.push_back({KDC, 3500000, 0, 0});               // 35,000.00
    v.push_back(std::make_unique<ER>(100000, 4, 50000));         r.push_back({KER, 100000, 4, 50000});            // 3,000.00
    v.push_back(std::make_unique<OP>(30000, 60000));             r.push_back({KOP, 30000, 60000, 0});             // 900.00
    v.push_back(std::make_unique<IP>(2, 200000, 2050000));       r.push_back({KIP, 2, 200000, 2050000});          // 4,000+20,500 = 24,500.00
    v.push_back(std::make_unique<DC>(1200000));                  r.push_back({KDC, 1200000, 0, 0});               // 12,000.00
    v.push_back(std::make_unique<ER>(150000, 6, 50000));         r.push_back({KER, 150000, 6, 50000});            // 1,500+3,000 = 4,500.00
    v.push_back(std::make_unique<IP>(10, 300000, 4600000));      r.push_back({KIP, 10, 300000, 4600000});         // 30,000+46,000 = 76,000.00
    v.push_back(std::make_unique<OP>(25000, 100000));            r.push_back({KOP, 25000, 100000, 0});            // 1,250.00
    Paise sv = 0, ss = 0, sh = 0; int agree = 0;
    std::cout << "No  hand      virtual   switch\n";
    for (int i = 0; i < 10; ++i) {
        Paise a = v[i]->total(), b = totalSwitch(r[i]); sv += a; ss += b; sh += hand[i]; agree += (a == b && a == hand[i]);
        std::cout << (i < 9 ? " " : "") << i + 1 << "  " << hand[i] << "  " << a << "  " << b << (a == b && a == hand[i] ? "  ok" : "  DIFF") << '\n';
    }
    std::cout << "Sum hand=" << sh << " virtual=" << sv << " switch=" << ss << "  " << agree << "/10 agree\n";
}
