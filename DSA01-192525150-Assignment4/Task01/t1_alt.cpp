#include <chrono>
#include <iostream>
#include <string>
constexpr double DA_RATE = 0.42;
enum class Kind { Professor, LabAssistant, GuestFaculty };
struct StaffAll {                                // one class, a type field, every field for every kind
    Kind kind; std::string name; int id;
    double basic, research, hours, rate;
    double pay() const {
        switch (kind) {
        case Kind::Professor:    return basic + basic * DA_RATE + research;
        case Kind::LabAssistant: return basic + basic * DA_RATE;
        case Kind::GuestFaculty: return hours * rate;
        }
        return 0;
    }
};
class Base { public: virtual ~Base() = default; virtual double pay() const = 0; };
class P : public Base { std::string name; int id; double basic, research; public: P(double b, double r) : name("x"), id(1), basic(b), research(r) {} double pay() const override { return basic + basic * DA_RATE + research; } };
class L : public Base { std::string name; int id; double basic; public: explicit L(double b) : name("x"), id(1), basic(b) {} double pay() const override { return basic + basic * DA_RATE; } };
class G : public Base { std::string name; int id; double hours, rate; public: G(double h, double r) : name("x"), id(1), hours(h), rate(r) {} double pay() const override { return hours * rate; } };
int main()
{
    std::cout << "sizeof single class " << sizeof(StaffAll) << ", sizeof P/L/G " << sizeof(P) << '/' << sizeof(L) << '/' << sizeof(G) << '\n';
    const int N = 3000;
    StaffAll a[N]; Base* b[N];
    for (int i = 0; i < N; ++i) {
        int k = i % 3;
        a[i] = StaffAll{static_cast<Kind>(k), "x", i, 30000.0 + i, 5000, 20, 1500};
        b[i] = k == 0 ? static_cast<Base*>(new P(30000 + i, 5000)) : k == 1 ? static_cast<Base*>(new L(30000 + i)) : static_cast<Base*>(new G(20, 1500));
    }
    const int REP = 30000; volatile double sink = 0; (void)sink;
    auto t0 = std::chrono::steady_clock::now();
    double s1 = 0; for (int r = 0; r < REP; ++r) for (int i = 0; i < N; ++i) s1 += a[i].pay();
    auto t1 = std::chrono::steady_clock::now();
    double s2 = 0; for (int r = 0; r < REP; ++r) for (int i = 0; i < N; ++i) s2 += b[i]->pay();
    auto t2 = std::chrono::steady_clock::now();
    sink = s1 + s2;
    std::cout << "90 million pay() calls: switch " << std::chrono::duration<double, std::milli>(t1 - t0).count() << " ms, virtual " << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms, equal totals " << (s1 == s2) << '\n';
    for (int i = 0; i < N; ++i) delete b[i];
}
