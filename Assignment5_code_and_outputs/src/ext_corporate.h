// ext_corporate.h : NEW file. Adds one bill type and one payer WITHOUT editing hospital.h or core_tests.h
#pragma once
#include "hospital.h"
class DialysisBill : public Bill {                 // new bill type: sessions x per-session charge + consumables
    int sessions; Paise perSession, consumables;
public:
    DialysisBill(std::string p, int n, Paise rate, Paise cons)
        : Bill(std::move(p)), sessions(n), perSession(nonNeg(rate, "session charge")), consumables(nonNeg(cons, "consumables")) {
        if (n < 0) throw NegativeChargeError("negative sessions");
    }
    Paise total() const override { return sessions * perSession + consumables; }
    std::string type() const override { return "DL"; }
    std::string packageCode() const override { return "DL-STD"; }
};
class CorporatePayer : public Payer {              // new payer: employer pays a flat percentage up to a yearly budget
    int percent; Paise budgetLeft;
public:
    CorporatePayer(int pct, Paise budget) : percent(pct), budgetLeft(nonNeg(budget, "budget")) {}
    Paise amountCovered(const Bill& b) const override {
        Paise c = b.total() * percent / 100;
        if (c > budgetLeft) throw CoverExceededError("corporate budget exhausted: needs " + rs(c) + ", left " + rs(budgetLeft));
        return c;
    }
    std::string name() const override { return "Corporate"; }
};
