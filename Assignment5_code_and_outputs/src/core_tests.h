// core_tests.h : the earlier (B1) tests, reusable so B4 can prove they still pass
#pragma once
#include "hospital.h"
#include <iostream>
inline int runCoreTests(bool verbose = true) {
    OutPatientBill op("Ravi", 50000, 120000);                       // 1,700.00
    InPatientBill  ip("Meena", 3, 250000, 4000000, 150000);         // 3*2500+40000+1500 = 49,000.00
    DayCareBill    dc("Kiran", "DC-CATARACT", 3500000);             // 35,000.00
    EmergencyBill  er("Asha", 100000, 4, 50000);                    // 1000 + 4*500 = 3,000.00
    SelfPay self; Insurance ins(5000000, 20);                       // limit 50,000.00, copay 20%
    GovernmentScheme gov({{"OP-STD", 80000}, {"IP-GEN", 3000000}, {"DC-CATARACT", 2500000}, {"ER-STD", 200000}});
    struct T { const Bill* b; const Payer* p; Paise total, covered, pays; const char* label; };
    T tests[] = {
        {&op, &self, 170000, 0, 170000, "OP + Self"},
        {&op, &ins,  170000, 136000, 34000, "OP + Insurance"},
        {&op, &gov,  170000, 80000, 90000, "OP + Govt"},
        {&ip, &self, 4900000, 0, 4900000, "IP + Self"},
        {&ip, &ins,  4900000, 3920000, 980000, "IP + Insurance"},
        {&ip, &gov,  4900000, 3000000, 1900000, "IP + Govt"},
        {&dc, &self, 3500000, 0, 3500000, "DC + Self"},
        {&dc, &ins,  3500000, 2800000, 700000, "DC + Insurance"},
        {&dc, &gov,  3500000, 2500000, 1000000, "DC + Govt"},
        {&er, &self, 300000, 0, 300000, "ER + Self"},
        {&er, &ins,  300000, 240000, 60000, "ER + Insurance"},
        {&er, &gov,  300000, 200000, 100000, "ER + Govt"},
    };
    int pass = 0, n = 0;
    for (auto& t : tests) {
        Settlement s = settle(*t.b, *t.p);
        bool ok = s.total == t.total && s.covered == t.covered && s.patientPays == t.pays;
        ++n; pass += ok;
        if (verbose) std::cout << (n < 10 ? " " : "") << n << "  " << t.label << std::string(16 - std::string(t.label).size(), ' ')
            << rs(t.total) << "  " << rs(t.covered) << "  " << rs(t.pays) << "   "
            << rs(s.total) << "  " << rs(s.covered) << "  " << rs(s.patientPays) << "   " << (ok ? "PASS" : "FAIL") << '\n';
    }
    return pass == n ? 0 : 1;
}
