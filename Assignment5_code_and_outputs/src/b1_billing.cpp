#include "core_tests.h"
int main() {
    std::cout << "No  Case            Exp.tot   Exp.cov   Exp.pay   Act.tot   Act.cov   Act.pay\n";
    int rc = runCoreTests();
    // day total through base-class pointers
    std::vector<std::unique_ptr<Bill>> day;
    day.push_back(std::make_unique<OutPatientBill>("Ravi", 50000, 120000));
    day.push_back(std::make_unique<InPatientBill>("Meena", 3, 250000, 4000000, 150000));
    day.push_back(std::make_unique<DayCareBill>("Kiran", "DC-CATARACT", 3500000));
    day.push_back(std::make_unique<EmergencyBill>("Asha", 100000, 4, 50000));
    Paise sum = 0;
    for (const auto& b : day) { std::cout << b->type() << ' ' << b->who() << ": " << rs(b->total()) << '\n'; sum += b->total(); }
    std::cout << "Day total " << rs(sum) << (sum == 8870000 ? "  (matches hand total 88,700.00)" : "  MISMATCH") << '\n';
    return rc;
}
