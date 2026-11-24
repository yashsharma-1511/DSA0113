// hospital.h : bills, payers and exceptions (money in whole paise, long long)
#pragma once
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using Paise = long long;
inline std::string rs(Paise p) {              // 1700000 -> "17000.00"
    std::string s = std::to_string(p / 100) + ".";
    int q = static_cast<int>(p % 100); if (q < 0) q = -q;
    return s + (q < 10 ? "0" : "") + std::to_string(q);
}

// ---------- exception hierarchy ----------
class BillingError : public std::runtime_error { public: using std::runtime_error::runtime_error; };
class NegativeChargeError : public BillingError { public: using BillingError::BillingError; };
class MissingRecordError  : public BillingError { public: using BillingError::BillingError; };
class ClaimError          : public BillingError { public: using BillingError::BillingError; };
class CoverExceededError  : public ClaimError   { public: using ClaimError::ClaimError; };
class UnknownPackageError : public ClaimError   { public: using ClaimError::ClaimError; };

inline Paise nonNeg(Paise v, const char* what) {
    if (v < 0) throw NegativeChargeError(std::string("negative ") + what);
    return v;
}

// ---------- Bill hierarchy ----------
class Bill {
protected:
    std::string patient;
public:
    explicit Bill(std::string p) : patient(std::move(p)) {}
    virtual ~Bill() = default;
    virtual Paise total() const = 0;                 // pure virtual
    virtual std::string type() const = 0;            // pure virtual
    virtual std::string packageCode() const = 0;     // key into government rate table
    const std::string& who() const { return patient; }
};
class OutPatientBill : public Bill {
    Paise consultation, tests;
public:
    OutPatientBill(std::string p, Paise c, Paise t)
        : Bill(std::move(p)), consultation(nonNeg(c, "consultation")), tests(nonNeg(t, "tests")) {}
    Paise total() const override { return consultation + tests; }
    std::string type() const override { return "OP"; }
    std::string packageCode() const override { return "OP-STD"; }
};
class InPatientBill : public Bill {
    int days; Paise perDay, procedures, medicines;
public:
    InPatientBill(std::string p, int d, Paise rate, Paise proc, Paise med)
        : Bill(std::move(p)), days(d), perDay(nonNeg(rate, "room rate")),
          procedures(nonNeg(proc, "procedures")), medicines(nonNeg(med, "medicines")) {
        if (d < 0) throw NegativeChargeError("negative days");
    }
    Paise total() const override { return days * perDay + procedures + medicines; }
    std::string type() const override { return "IP"; }
    std::string packageCode() const override { return "IP-GEN"; }
};
class DayCareBill : public Bill {
    std::string code; Paise packagePrice;
public:
    DayCareBill(std::string p, std::string c, Paise price)
        : Bill(std::move(p)), code(std::move(c)), packagePrice(nonNeg(price, "package price")) {}
    Paise total() const override { return packagePrice; }
    std::string type() const override { return "DC"; }
    std::string packageCode() const override { return code; }
};
class EmergencyBill : public Bill {
    Paise base; int hours; Paise perHour;
public:
    EmergencyBill(std::string p, Paise b, int h, Paise r)
        : Bill(std::move(p)), base(nonNeg(b, "base charge")), hours(h), perHour(nonNeg(r, "hourly charge")) {
        if (h < 0) throw NegativeChargeError("negative hours");
    }
    Paise total() const override { return base + hours * perHour; }
    std::string type() const override { return "ER"; }
    std::string packageCode() const override { return "ER-STD"; }
};

// ---------- Payer hierarchy ----------
class Payer {
public:
    virtual ~Payer() = default;
    virtual Paise amountCovered(const Bill& b) const = 0;   // pure virtual
    virtual std::string name() const = 0;
};
class SelfPay : public Payer {
public:
    Paise amountCovered(const Bill&) const override { return 0; }
    std::string name() const override { return "Self"; }
};
class Insurance : public Payer {
    Paise coverLimit; int copayPercent;
public:
    Insurance(Paise limit, int copay) : coverLimit(nonNeg(limit, "cover limit")), copayPercent(copay) {
        if (copay < 0 || copay > 100) throw ClaimError("co-payment must be 0..100");
    }
    Paise amountCovered(const Bill& b) const override {
        Paise claim = b.total() * (100 - copayPercent) / 100;   // insurer's share
        if (claim > coverLimit) throw CoverExceededError("claim " + rs(claim) + " exceeds cover " + rs(coverLimit));
        return claim;
    }
    std::string name() const override { return "Insurance"; }
};
class GovernmentScheme : public Payer {
    std::map<std::string, Paise> rates;       // package code -> fixed rate
public:
    explicit GovernmentScheme(std::map<std::string, Paise> r) : rates(std::move(r)) {}
    Paise amountCovered(const Bill& b) const override {
        auto it = rates.find(b.packageCode());
        if (it == rates.end()) throw UnknownPackageError("unknown package " + b.packageCode());
        return it->second < b.total() ? it->second : b.total();  // scheme pays the fixed rate, at most the bill
    }
    std::string name() const override { return "Govt"; }
};

// patient registry (missing record error)
class Registry {
    std::map<std::string, int> ids;
public:
    void add(const std::string& n, int id) { ids[n] = id; }
    int find(const std::string& n) const {
        auto it = ids.find(n);
        if (it == ids.end()) throw MissingRecordError("no record for " + n);
        return it->second;
    }
};

struct Settlement { Paise total, covered, patientPays; };
inline Settlement settle(const Bill& b, const Payer& p) {
    Paise t = b.total(), c = p.amountCovered(b);
    return {t, c, t - c};
}
