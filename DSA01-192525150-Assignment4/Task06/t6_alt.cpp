#include <chrono>
#include <iostream>
#include <string>
constexpr long long FUEL_RATE = 100, KWH_RATE = 12; constexpr int LATE_PERCENT = 50, EXTRA_SEAT_FEE = 50;
enum class Type { Bicycle, TwoWheeler, Car, ElectricCar };
struct VehicleAll {                                      // one class, a type code, every field for every type
    Type type; std::string reg; long long dailyRate; bool rented; int bookedDays;
    int level; int seats; int batteryKwh; int dueDay, dueMonth, dueYear;
    long long charge(int days, int used) const {
        switch (type) {
        case Type::Bicycle:    return days * dailyRate;
        case Type::TwoWheeler: return days * dailyRate + used * FUEL_RATE;
        case Type::Car:        return days * dailyRate + used * FUEL_RATE + (seats > 4 ? (seats - 4) * EXTRA_SEAT_FEE * days : 0LL);
        case Type::ElectricCar:return days * dailyRate + used * KWH_RATE + (seats > 4 ? (seats - 4) * EXTRA_SEAT_FEE * days : 0LL);
        }
        return 0;
    }
};
struct VB { virtual ~VB() = default; virtual long long charge(int, int) const = 0; };
struct VBic : VB { std::string reg{"x"}; long long rate; bool rented = false; int booked = 0; explicit VBic(long long r) : rate(r) {} long long charge(int d, int) const override { return d * rate; } };
struct VMot : VB { std::string reg{"x"}; long long rate; bool rented = false; int booked = 0; int level = 0; int due[3] = {1, 1, 2027}; explicit VMot(long long r) : rate(r) {} };
struct V2 : VMot { using VMot::VMot; long long charge(int d, int u) const override { return d * rate + u * FUEL_RATE; } };
struct VCar : VMot { int seats = 5; using VMot::VMot; long long charge(int d, int u) const override { return d * rate + u * FUEL_RATE + (seats > 4 ? (seats - 4) * EXTRA_SEAT_FEE * d : 0LL); } };
struct VEv : VCar { int batt = 60; using VCar::VCar; long long charge(int d, int u) const override { return d * rate + u * KWH_RATE + (seats > 4 ? (seats - 4) * EXTRA_SEAT_FEE * d : 0LL); } };
int main()
{
    std::cout << "sizeof single class " << sizeof(VehicleAll) << "; Bicycle " << sizeof(VBic) << ", TwoWheeler " << sizeof(V2) << ", Car " << sizeof(VCar) << ", ElectricCar " << sizeof(VEv) << " (each includes an 8-byte vtable pointer)\n";
    const int N = 4000; VehicleAll a[N]; VB* b[N];
    for (int i = 0; i < N; ++i) {
        a[i] = VehicleAll{static_cast<Type>(i % 4), "x", 1000, false, 0, 10, 5, 60, 1, 1, 2027};
        switch (i % 4) { case 0: b[i] = new VBic(1000); break; case 1: b[i] = new V2(1000); break; case 2: b[i] = new VCar(1000); break; default: b[i] = new VEv(1000); }
    }
    const int REP = 20000; long long s1 = 0, s2 = 0; volatile long long sink = 0;
    auto t0 = std::chrono::steady_clock::now();
    for (int r = 0; r < REP; ++r) for (int i = 0; i < N; ++i) s1 += a[i].charge(3, 20);
    auto t1 = std::chrono::steady_clock::now();
    for (int r = 0; r < REP; ++r) for (int i = 0; i < N; ++i) s2 += b[i]->charge(3, 20);
    auto t2 = std::chrono::steady_clock::now();
    sink = s1 + s2; (void)sink;
    std::cout << "80 million charge() calls: switch " << std::chrono::duration<double, std::milli>(t1 - t0).count() << " ms, virtual " << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms, same totals " << (s1 == s2) << '\n';
    for (int i = 0; i < N; ++i) delete b[i];
}
