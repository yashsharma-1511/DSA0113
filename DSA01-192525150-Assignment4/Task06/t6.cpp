#include "vehicle.h"
int main()
{
    Fleet fleet;
    fleet.add(new Bicycle("B-101", 100));
    fleet.add(new TwoWheeler("T-202", 300, 8, Date(15, 12, 2026)));
    fleet.add(new Car("C-303", 1500, 40, Date(1, 1, 2027), 7));
    fleet.add(new ElectricCar("E-404", 2000, 60, Date(20, 12, 2026), 5, 60));
    std::cout << "--- fleet before renting ---\n"; fleet.list();
    struct Rental { const char* reg; int booked, taken, used; };
    Rental r[] = { {"B-101", 2, 2, 0}, {"T-202", 2, 3, 4}, {"C-303", 3, 3, 20}, {"E-404", 2, 4, 25} };
    long long revenue = 0;
    std::cout << "--- renting ---\n";
    for (const Rental& x : r) std::cout << x.reg << " rent for " << x.booked << " days: " << (fleet.find(x.reg)->rent(x.booked) ? "ok" : "refused") << '\n';
    std::cout << "B-101 rented again: " << (fleet.find("B-101")->rent(1) ? "ok" : "refused") << '\n';
    std::cout << "--- returning ---\n";
    for (const Rental& x : r) {
        Vehicle* v = fleet.find(x.reg);
        long long use = v->usageCharge(x.taken, x.used), late = v->lateCharge(x.taken);   // before the return resets the booking
        long long bill = v->giveBack(x.taken, x.used);
        std::cout << std::left << std::setw(6) << x.reg << std::right << " taken " << x.taken << " (booked " << x.booked << "), used " << std::setw(2) << x.used
                  << "  usage Rs " << std::setw(5) << use << "  late Rs " << std::setw(4) << late << "  bill Rs " << bill << '\n';
        revenue += bill;
    }
    std::cout << "B-101 returned again: " << fleet.find("B-101")->giveBack(1, 0) << " (-1 = not rented)\n";
    std::cout << "unknown vehicle: " << (fleet.find("Z-999") == nullptr ? "not found" : "found") << '\n';
    std::cout << "total revenue Rs " << revenue << '\n';
    std::cout << "--- fleet after returns ---\n"; fleet.list();
}
