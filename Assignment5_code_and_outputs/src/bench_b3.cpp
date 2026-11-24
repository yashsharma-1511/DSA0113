// B3.1/B3.2 + variant alternative: virtual vs switch vs std::variant, -O2
#include <chrono>
#include <cstdio>
#include <cstdint>
#include <memory>
#include <variant>
#include <vector>
#include <algorithm>
using Clock = std::chrono::steady_clock;
using Paise = long long;
struct Spec { int type; Paise a, b, c; };           // data for one bill
static Spec makeSpec(uint64_t i) {                   // deterministic pseudo-random mix of the 4 types
    uint64_t x = i * 6364136223846793005ULL + 1442695040888963407ULL; int t = (x >> 33) & 3;
    return {t, (Paise)(100 + (x >> 40) % 900), (Paise)(1 + (x >> 20) % 9), (Paise)(50 + (x >> 10) % 500)};
}
// (a) virtual
struct Bill { virtual ~Bill() = default; virtual Paise total() const = 0; };
struct OP : Bill { Paise c, t; OP(Paise c, Paise t) : c(c), t(t) {} Paise total() const override { return c + t; } };
struct IP : Bill { int d; Paise r, p; IP(int d, Paise r, Paise p) : d(d), r(r), p(p) {} Paise total() const override { return d * r + p; } };
struct DC : Bill { Paise pk; explicit DC(Paise p) : pk(p) {} Paise total() const override { return pk; } };
struct ER : Bill { Paise b; int h; Paise r; ER(Paise b, int h, Paise r) : b(b), h(h), r(r) {} Paise total() const override { return b + h * r; } };
static std::unique_ptr<Bill> mk(const Spec& s) {
    switch (s.type) { case 0: return std::make_unique<OP>(s.a, s.c); case 1: return std::make_unique<IP>((int)s.b, s.a, s.c);
                      case 2: return std::make_unique<DC>(s.a + s.c); default: return std::make_unique<ER>(s.a, (int)s.b, s.c); }
}
// (b) struct + switch
struct Rec { int type; Paise a, b, c; };
static inline Paise totalSwitch(const Rec& r) {
    switch (r.type) { case 0: return r.a + r.c; case 1: return r.b * r.a + r.c; case 2: return r.a + r.c; default: return r.a + r.b * r.c; }
}
// (c) variant
struct VOP { Paise c, t; }; struct VIP { int d; Paise r, p; }; struct VDC { Paise pk; }; struct VER { Paise b; int h; Paise r; };
using Var = std::variant<VOP, VIP, VDC, VER>;
struct TotalVisitor {
    Paise operator()(const VOP& x) const { return x.c + x.t; }
    Paise operator()(const VIP& x) const { return x.d * x.r + x.p; }
    Paise operator()(const VDC& x) const { return x.pk; }
    Paise operator()(const VER& x) const { return x.b + x.h * x.r; }
};
static Var mkv(const Spec& s) {
    switch (s.type) { case 0: return VOP{s.a, s.c}; case 1: return VIP{(int)s.b, s.a, s.c}; case 2: return VDC{s.a + s.c}; default: return VER{s.a, (int)s.b, s.c}; }
}
template <class F> double bestMs(int reps, F f) {          // best-of-5 timing, ms per pass
    double best = 1e18;
    for (int k = 0; k < 5; ++k) { auto t0 = Clock::now(); for (int i = 0; i < reps; ++i) f(); auto t1 = Clock::now();
        best = std::min(best, std::chrono::duration<double, std::milli>(t1 - t0).count() / reps); }
    return best;
}
volatile Paise sink;
int main() {
    std::printf("sizeof: Bill-derived max=%zu  Rec=%zu  Var=%zu\n", sizeof(ER), sizeof(Rec), sizeof(Var));
    const size_t sizes[] = {1000, 1000000, 10000000}; const int reps[] = {2000, 20, 3};
    for (int s = 0; s < 3; ++s) {
        size_t n = sizes[s]; Paise ta = 0, tb = 0, tc = 0; double ma, mb, mc;
        { std::vector<std::unique_ptr<Bill>> v; v.reserve(n); for (size_t i = 0; i < n; ++i) v.push_back(mk(makeSpec(i)));
          ma = bestMs(reps[s], [&] { Paise t = 0; for (auto& b : v) t += b->total(); sink = t; ta = t; }); }
        { std::vector<Rec> v(n); for (size_t i = 0; i < n; ++i) { Spec sp = makeSpec(i); v[i] = {sp.type, sp.a, sp.b, sp.c}; }
          // the Rec layout above mirrors mk(): type 2 uses a+c
          mb = bestMs(reps[s], [&] { Paise t = 0; for (auto& r : v) t += totalSwitch(r); sink = t; tb = t; }); }
        { std::vector<Var> v; v.reserve(n); for (size_t i = 0; i < n; ++i) v.push_back(mkv(makeSpec(i)));
          mc = bestMs(reps[s], [&] { Paise t = 0; for (auto& x : v) t += std::visit(TotalVisitor{}, x); sink = t; tc = t; }); }
        std::printf("n=%zu virtual=%.4f ms switch=%.4f ms variant=%.4f ms | totals %lld %lld %lld %s\n", n, ma, mb, mc, ta, tb, tc, (ta == tb && tb == tc) ? "AGREE" : "DIFFER");
        std::fflush(stdout);
    }
}
