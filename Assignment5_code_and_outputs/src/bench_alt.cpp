// Alternatives: dynamic_cast chain (A2), std::function payers (A3), templates (B1), rule-table plug-ins (B4), -O2
#include <chrono>
#include <cstdio>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
using Clock = std::chrono::steady_clock; using Paise = long long;
volatile Paise sink;
template <class F> double bestMs(F f) { double b = 1e18; for (int k = 0; k < 5; ++k) { auto t0 = Clock::now(); f(); auto t1 = Clock::now(); b = std::min(b, std::chrono::duration<double, std::milli>(t1 - t0).count()); } return b; }
static uint64_t lcg(uint64_t i) { return i * 6364136223846793005ULL + 1442695040888963407ULL; }
struct Bill { virtual ~Bill() = default; virtual Paise total() const = 0; };
struct OP : Bill { Paise c, t; OP(Paise c, Paise t) : c(c), t(t) {} Paise total() const override { return c + t; } };
struct IP : Bill { int d; Paise r, p; IP(int d, Paise r, Paise p) : d(d), r(r), p(p) {} Paise total() const override { return d * r + p; } };
struct DC : Bill { Paise pk; explicit DC(Paise p) : pk(p) {} Paise total() const override { return pk; } };
struct ER : Bill { Paise b; int h; Paise r; ER(Paise b, int h, Paise r) : b(b), h(h), r(r) {} Paise total() const override { return b + h * r; } };
static std::unique_ptr<Bill> mk(uint64_t i) { uint64_t x = lcg(i); int t = (x >> 33) & 3; Paise a = 100 + (x >> 40) % 900, c = 50 + (x >> 10) % 500; int b = 1 + (x >> 20) % 9;
    switch (t) { case 0: return std::make_unique<OP>(a, c); case 1: return std::make_unique<IP>(b, a, c); case 2: return std::make_unique<DC>(a + c); default: return std::make_unique<ER>(a, b, c); } }
// non-virtual versions for the dynamic_cast demo: base has no total(), caller tests the type
struct NBill { virtual ~NBill() = default; };      // polymorphic only so dynamic_cast works
struct NOP : NBill { Paise c, t; NOP(Paise c, Paise t) : c(c), t(t) {} };
struct NIP : NBill { int d; Paise r, p; NIP(int d, Paise r, Paise p) : d(d), r(r), p(p) {} };
struct NDC : NBill { Paise pk; explicit NDC(Paise p) : pk(p) {} };
struct NER : NBill { Paise b; int h; Paise r; NER(Paise b, int h, Paise r) : b(b), h(h), r(r) {} };
static std::unique_ptr<NBill> mkn(uint64_t i) { uint64_t x = lcg(i); int t = (x >> 33) & 3; Paise a = 100 + (x >> 40) % 900, c = 50 + (x >> 10) % 500; int b = 1 + (x >> 20) % 9;
    switch (t) { case 0: return std::make_unique<NOP>(a, c); case 1: return std::make_unique<NIP>(b, a, c); case 2: return std::make_unique<NDC>(a + c); default: return std::make_unique<NER>(a, b, c); } }
static Paise totalCast(const NBill* b) {
    if (auto p = dynamic_cast<const NOP*>(b)) return p->c + p->t;
    if (auto p = dynamic_cast<const NIP*>(b)) return p->d * p->r + p->p;
    if (auto p = dynamic_cast<const NDC*>(b)) return p->pk;
    if (auto p = dynamic_cast<const NER*>(b)) return p->b + p->h * p->r;
    return 0;
}
// payers
struct Payer { virtual ~Payer() = default; virtual Paise covered(Paise total) const = 0; };
struct Ins : Payer { Paise limit; int copay; Ins(Paise l, int c) : limit(l), copay(c) {} Paise covered(Paise t) const override { Paise c = t * (100 - copay) / 100; return c > limit ? limit : c; } };
// templates
struct TOP { Paise c, t; Paise total() const { return c + t; } };
struct TIP { int d; Paise r, p; Paise total() const { return d * r + p; } };
struct TDC { Paise pk; Paise total() const { return pk; } };
struct TER { Paise b; int h; Paise r; Paise total() const { return b + h * r; } };
template <class T> Paise sumAll(const std::vector<T>& v) { Paise s = 0; for (const T& x : v) s += x.total(); return s; }
// rule table (plug-in): rules loaded as data, interpreted generically
struct Rule { int kind; Paise a, b, c; };
static Paise interpret(const Rule& r, Paise x, Paise y, Paise z) { switch (r.kind) { case 0: return x + z; case 1: return y * x + z; case 2: return x + z; default: return x + y * z; } }
int main() {
    const int N = 1000000;
    { // A2
        std::vector<std::unique_ptr<Bill>> v; std::vector<std::unique_ptr<NBill>> w; for (int i = 0; i < N; ++i) { v.push_back(mk(i)); w.push_back(mkn(i)); }
        Paise a = 0, b = 0;
        double tv = bestMs([&] { Paise s = 0; for (auto& p : v) s += p->total(); sink = s; a = s; });
        double tc = bestMs([&] { Paise s = 0; for (auto& p : w) s += totalCast(p.get()); sink = s; b = s; });
        std::printf("A2 n=%d virtual=%.2f ms (%.2f ns/bill)  dynamic_cast chain(k=4)=%.2f ms (%.2f ns/bill) agree=%d  sizeof(NBill-with-vptr)=%zu sizeof(plain int-less struct)=1\n", N, tv, tv * 1e6 / N, tc, tc * 1e6 / N, a == b, sizeof(NBill));
        std::printf("A2 sizeof: OP(with vptr)=%zu  TOP(no vptr)=%zu\n", sizeof(OP), sizeof(TOP));
    }
    { // A3
        Ins ins(500000, 20); const Payer* pp = &ins; std::function<Paise(Paise)> fn = [limit = (Paise)500000, copay = 20](Paise t) { Paise c = t * (100 - copay) / 100; return c > limit ? limit : c; };
        Paise (*fp)(Paise) = [](Paise t) { Paise c = t * 80 / 100; return c > 500000 ? (Paise)500000 : c; };
        std::vector<Paise> t(N); for (int i = 0; i < N; ++i) t[i] = 1000 + (lcg(i) >> 40) % 900000;
        double a = bestMs([&] { Paise s = 0; for (Paise x : t) s += pp->covered(x); sink = s; });
        double b = bestMs([&] { Paise s = 0; for (Paise x : t) s += fn(x); sink = s; });
        double c = bestMs([&] { Paise s = 0; for (Paise x : t) s += fp(x); sink = s; });
        std::printf("A3 n=%d abstract Payer=%.2f ms  std::function=%.2f ms  function pointer=%.2f ms | sizeof(Ins)=%zu sizeof(std::function)=%zu sizeof(fnptr)=%zu\n", N, a, b, c, sizeof(Ins), sizeof(fn), sizeof(fp));
    }
    { // B1 templates
        std::vector<std::unique_ptr<Bill>> v; std::vector<TOP> o; std::vector<TIP> ip; std::vector<TDC> d; std::vector<TER> e;
        for (int i = 0; i < N; ++i) { uint64_t x = lcg(i); int t = (x >> 33) & 3; Paise a = 100 + (x >> 40) % 900, c = 50 + (x >> 10) % 500; int b = 1 + (x >> 20) % 9; v.push_back(mk(i));
            switch (t) { case 0: o.push_back({a, c}); break; case 1: ip.push_back({b, a, c}); break; case 2: d.push_back({a + c}); break; default: e.push_back({a, b, c}); } }
        Paise r1 = 0, r2 = 0;
        double tv = bestMs([&] { Paise s = 0; for (auto& p : v) s += p->total(); sink = s; r1 = s; });
        double tt = bestMs([&] { Paise s = sumAll(o) + sumAll(ip) + sumAll(d) + sumAll(e); sink = s; r2 = s; });
        std::printf("B1 n=%d virtual(one list)=%.2f ms  templates(4 containers)=%.2f ms  agree=%d\n", N, tv, tt, r1 == r2);
    }
    { // B4 plug-in rules: 10,000,000 bills
        const int M = 10000000; std::vector<Rule> rules = {{0, 0, 0, 0}, {1, 0, 0, 0}, {2, 0, 0, 0}, {3, 0, 0, 0}};
        std::unordered_map<std::string, int> byName = {{"OP-STD", 0}, {"IP-GEN", 1}, {"DC-PKG", 2}, {"ER-STD", 3}};
        const char* names[] = {"OP-STD", "IP-GEN", "DC-PKG", "ER-STD"};
        struct D { int t; Paise x, y, z; }; std::vector<D> data(M); for (int i = 0; i < M; ++i) { uint64_t x = lcg(i); data[i] = {(int)((x >> 33) & 3), 100 + (Paise)((x >> 40) % 900), 1 + (Paise)((x >> 20) % 9), 50 + (Paise)((x >> 10) % 500)}; }
        std::vector<std::unique_ptr<Bill>> v; v.reserve(M); for (int i = 0; i < M; ++i) v.push_back(mk(i));
        Paise s1 = 0, s2 = 0, s3 = 0;
        double tv = bestMs([&] { Paise s = 0; for (auto& p : v) s += p->total(); sink = s; s1 = s; });
        double ti = bestMs([&] { Paise s = 0; for (const D& d : data) s += interpret(rules[d.t], d.x, d.y, d.z); sink = s; s2 = s; });
        double ts = bestMs([&] { Paise s = 0; for (const D& d : data) s += interpret(rules[byName.find(names[d.t])->second], d.x, d.y, d.z); sink = s; s3 = s; });
        std::printf("B4 n=%d virtual=%.1f ms  rule-table(index)=%.1f ms  rule-table(string key lookup)=%.1f ms  sizeof(Rule)=%zu\n", M, tv, ti, ts, sizeof(Rule));
    }
}
