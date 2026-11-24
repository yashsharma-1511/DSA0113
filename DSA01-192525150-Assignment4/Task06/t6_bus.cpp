#include "vehicle.h"                       // unchanged: the file is only included, never edited
class ElectricBus : public MotorVehicle {  // NEW type: only this class and one line in main are new
    int seats_;
public:
    ElectricBus(const std::string& reg, long long rate, int kwh, const Date& due, int seats) : MotorVehicle(reg, rate, kwh, due), seats_(seats) {}
    long long usageCharge(int days, int kwh) const override { return days * dailyRate() + kwh * KWH_RATE + days * 500; }   // Rs 500/day driver fee
    const char* kind() const override { return "ElectricBus"; }
    int seats() const { return seats_; }
};
int main()
{
    Fleet fleet;
    fleet.add(new Bicycle("B-101", 100));
    fleet.add(new ElectricCar("E-404", 2000, 60, Date(20, 12, 2026), 5, 60));
    fleet.add(new ElectricBus("EB-01", 6000, 200, Date(5, 1, 2027), 40));      // the extension
    fleet.list();
    fleet.find("EB-01")->rent(2);
    long long bill = fleet.find("EB-01")->giveBack(3, 120);                      // 1 late day
    std::cout << "ElectricBus EB-01: booked 2 days, returned after 3, used 120 kWh -> bill Rs " << bill << '\n';
    std::cout << "hand check: 3*6000 + 120*12 + 3*500 = " << 3 * 6000 + 120 * 12 + 3 * 500 << ", late 1 * 6000 * 50 / 100 = 3000, total " << 3 * 6000 + 120 * 12 + 3 * 500 + 3000 << '\n';
}
