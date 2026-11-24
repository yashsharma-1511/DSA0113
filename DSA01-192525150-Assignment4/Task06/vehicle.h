// vehicle.h : the existing fleet classes. A new vehicle type is added WITHOUT editing this file.
#pragma once
#include <iomanip>
#include <iostream>
#include <string>

constexpr long long FUEL_RATE = 100;       // Rs per litre
constexpr long long KWH_RATE = 12;         // Rs per kWh
constexpr int LATE_PERCENT = 50;           // each late day costs this % of the daily rate extra
constexpr int EXTRA_SEAT_FEE = 50;         // Rs per day for every seat above 4
constexpr int FLEET_CAPACITY = 16;

class Date {                               // member class: the service due date
    int d_, m_, y_;
public:
    Date(int d, int m, int y) : d_(d), m_(m), y_(y) {}
    std::string str() const { return std::to_string(d_) + "/" + std::to_string(m_) + "/" + std::to_string(y_); }
};

class Vehicle {                            // abstract base class
    std::string reg_;
    long long dailyRate_;
    bool rented_ = false;
    int bookedDays_ = 0;
protected:
    Vehicle(const std::string& reg, long long rate) : reg_(reg), dailyRate_(rate) {}
    long long dailyRate() const { return dailyRate_; }
public:
    Vehicle(const Vehicle&) = delete;
    Vehicle& operator=(const Vehicle&) = delete;
    virtual ~Vehicle() = default;                                           // needed: deleted through Vehicle*
    virtual long long usageCharge(int days, int used) const = 0;           // pure virtual: differs per type
    virtual const char* kind() const = 0;
    virtual void describe() const { std::cout << std::left << std::setw(12) << kind() << std::setw(8) << reg_ << std::right << "Rs " << std::setw(5) << dailyRate_ << "/day  "; }
    const std::string& reg() const { return reg_; }
    bool isRented() const { return rented_; }
    bool rent(int days) { if (rented_ || days <= 0) return false; rented_ = true; bookedDays_ = days; return true; }
    long long lateCharge(int daysTaken) const { return daysTaken > bookedDays_ ? static_cast<long long>(daysTaken - bookedDays_) * dailyRate_ * LATE_PERCENT / 100 : 0; }
    long long giveBack(int daysTaken, int used)                              // returns the total bill, -1 if not rented
    {
        if (!rented_) return -1;
        long long bill = usageCharge(daysTaken, used) + lateCharge(daysTaken);
        rented_ = false; bookedDays_ = 0;
        onReturn(used);
        return bill;
    }
protected:
    virtual void onReturn(int) {}                                            // hook: motor vehicles reduce their level
};

class Bicycle : public Vehicle {
public:
    Bicycle(const std::string& reg, long long rate) : Vehicle(reg, rate) {}
    long long usageCharge(int days, int) const override { return days * dailyRate(); }
    const char* kind() const override { return "Bicycle"; }
};

class MotorVehicle : public Vehicle {      // still abstract: usageCharge() is not defined here
    int level_;                            // litres of fuel, or kWh of charge
    Date serviceDue_;                      // member class
protected:
    MotorVehicle(const std::string& reg, long long rate, int level, const Date& due) : Vehicle(reg, rate), level_(level), serviceDue_(due) {}
    void onReturn(int used) override { level_ -= used; }
public:
    int level() const { return level_; }
    void describe() const override { Vehicle::describe(); std::cout << "level " << std::setw(3) << level_ << "  service due " << serviceDue_.str() << "  "; }
};

class TwoWheeler : public MotorVehicle {
public:
    TwoWheeler(const std::string& reg, long long rate, int litres, const Date& due) : MotorVehicle(reg, rate, litres, due) {}
    long long usageCharge(int days, int litres) const override { return days * dailyRate() + litres * FUEL_RATE; }
    const char* kind() const override { return "TwoWheeler"; }
};

class Car : public MotorVehicle {
    int seats_;
protected:
    long long seatFee(int days) const { return seats_ > 4 ? static_cast<long long>(seats_ - 4) * EXTRA_SEAT_FEE * days : 0; }
public:
    Car(const std::string& reg, long long rate, int litres, const Date& due, int seats) : MotorVehicle(reg, rate, litres, due), seats_(seats) {}
    long long usageCharge(int days, int litres) const override { return days * dailyRate() + litres * FUEL_RATE + seatFee(days); }
    const char* kind() const override { return "Car"; }
};

class ElectricCar : public Car {           // charged per kWh instead of per litre
    int batteryKwh_;
public:
    ElectricCar(const std::string& reg, long long rate, int kwh, const Date& due, int seats, int battery) : Car(reg, rate, kwh, due, seats), batteryKwh_(battery) {}
    long long usageCharge(int days, int kwh) const override { return days * dailyRate() + kwh * KWH_RATE + seatFee(days); }
    const char* kind() const override { return "ElectricCar"; }
    int battery() const { return batteryKwh_; }
};

class Fleet {                              // the fleet: an array of base-class pointers
    Vehicle* v_[FLEET_CAPACITY];
    int n_ = 0;
public:
    Fleet() = default;
    Fleet(const Fleet&) = delete;
    Fleet& operator=(const Fleet&) = delete;
    ~Fleet() { for (int i = 0; i < n_; ++i) delete v_[i]; }
    bool add(Vehicle* v) { if (n_ == FLEET_CAPACITY) return false; v_[n_++] = v; return true; }
    Vehicle* find(const std::string& reg) const { for (int i = 0; i < n_; ++i) if (v_[i]->reg() == reg) return v_[i]; return nullptr; }
    void list() const { for (int i = 0; i < n_; ++i) { v_[i]->describe(); std::cout << (v_[i]->isRented() ? "RENTED" : "available") << '\n'; } }
};
