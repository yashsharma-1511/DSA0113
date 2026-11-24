// Tickets created inside a function that is called twice
#include <iostream>
#include <string>
class BusTicket {
    static int issued;
    static const int CAPACITY = 3;
    int seat; std::string name;
public:
    explicit BusTicket(const std::string& n) : seat(0), name(n) { if (issued < CAPACITY) seat = ++issued; }
    ~BusTicket() { std::cout << "  (" << name << " destroyed)\n"; }
    static int count() { return issued; }
    void print() const { std::cout << name << ": " << (seat ? "Seat " + std::to_string(seat) : std::string("Waiting")) << '\n'; }
};
int BusTicket::issued = 0;
void bookTrip(const std::string& a, const std::string& b)
{
    BusTicket x(a), y(b);
    x.print(); y.print();
    std::cout << "  issued inside the function: " << BusTicket::count() << '\n';
}
int main()
{
    std::cout << "call 1\n"; bookTrip("Asha", "Bala");
    std::cout << "after call 1, Issued = " << BusTicket::count() << '\n';
    std::cout << "call 2\n"; bookTrip("Chitra", "Dev");
    std::cout << "after call 2, Issued = " << BusTicket::count() << '\n';
}
