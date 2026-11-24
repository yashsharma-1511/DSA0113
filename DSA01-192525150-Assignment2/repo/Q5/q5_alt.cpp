// Alternative: a separate TicketOffice object per bus (instead of a static counter) -> two buses at once
#include <chrono>
#include <iostream>
#include <string>
class TicketOffice { int issued = 0; int cap; public: explicit TicketOffice(int c) : cap(c) {} int next() { return issued < cap ? ++issued : 0; } int count() const { return issued; } };
class OfficeTicket { int seat; std::string name; public:
    OfficeTicket(const std::string& n, TicketOffice& o) : seat(o.next()), name(n) {}
    void print() const { std::cout << "  " << name << ": " << (seat ? "Seat " + std::to_string(seat) : std::string("Waiting")) << '\n'; } };
class StaticTicket { static int issued; static const int CAPACITY = 3; int seat = 0; std::string name; public:
    explicit StaticTicket(const std::string& n) : name(n) { if (issued < CAPACITY) seat = ++issued; }
    void print() const { std::cout << "  " << name << ": " << (seat ? "Seat " + std::to_string(seat) : std::string("Waiting")) << '\n'; } };
int StaticTicket::issued = 0;
int globalIssued = 0;                       // global-variable design
int takeGlobal() { return globalIssued < 3 ? ++globalIssued : 0; }
int main()
{
    std::cout << "Static counter, bus A (3 seats) then bus B (3 seats):\n";
    StaticTicket a1("A-Asha"), a2("A-Bala"), a3("A-Chitra"), b1("B-Dev"), b2("B-Esha");
    for (const StaticTicket* t : {&a1, &a2, &a3, &b1, &b2}) t->print();
    std::cout << "TicketOffice per bus, A has 3 seats, B has 2 seats:\n";
    TicketOffice busA(3), busB(2);
    OfficeTicket x1("A-Asha", busA), x2("A-Bala", busA), x3("A-Chitra", busA), y1("B-Dev", busB), y2("B-Esha", busB), y3("B-Farid", busB);
    for (const OfficeTicket* t : {&x1, &x2, &x3, &y1, &y2, &y3}) t->print();
    std::cout << "busA issued " << busA.count() << ", busB issued " << busB.count() << '\n';
    std::cout << "sizeof(StaticTicket)=" << sizeof(StaticTicket) << " sizeof(OfficeTicket)=" << sizeof(OfficeTicket) << " (office reference not stored)\n";
    // timing 10,000,000 seat requests
    const int N = 10000000; volatile long long sink = 0; (void)sink;
    auto t0 = std::chrono::steady_clock::now(); { long long s = 0; for (int i = 0; i < N; ++i) { StaticTicket t("x"); s += 1; } sink = s; }
    auto t1 = std::chrono::steady_clock::now(); { TicketOffice o(3); long long s = 0; for (int i = 0; i < N; ++i) { OfficeTicket t("x", o); s += 1; } sink = s; }
    auto t2 = std::chrono::steady_clock::now(); { long long s = 0; for (int i = 0; i < N; ++i) { s += takeGlobal(); } sink = s; }
    auto t3 = std::chrono::steady_clock::now();
    auto ms = [](auto a, auto b){ return std::chrono::duration<double, std::milli>(b - a).count(); };
    std::cout << "10M tickets: static " << ms(t0,t1) << " ms, TicketOffice " << ms(t1,t2) << " ms, global counter only " << ms(t2,t3) << " ms\n";
}
