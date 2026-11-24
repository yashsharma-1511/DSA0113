#include "core_tests.h"
#include "ext_corporate.h"
#include <iostream>
#include <new>
// A5 design: inner level logs and rethrows with "throw;", batch level records the rejection, top level reports
Settlement settleLogged(const Bill& b, const Payer& p) {
    try { return settle(b, p); }
    catch (const ClaimError& e) { std::cout << "  [audit] " << b.type() << ' ' << b.who() << ": " << e.what() << '\n'; throw; }
}
int main() {
    int failures = 0;
    std::cout << "=== 1. earlier B1 tests (core_tests.h, unchanged) ===\n";
    int rc = runCoreTests(false); std::cout << (rc == 0 ? "all 12 earlier tests PASS\n" : "EARLIER TESTS FAIL\n"); failures += rc;

    std::cout << "\n=== 2. new bill type and new payer ===\n";
    DialysisBill dl("Hari", 4, 250000, 80000);               // 4 x 2,500 + 800 = 10,800.00
    CorporatePayer corp(70, 2000000);                        // 70%, budget 20,000.00 -> covers 7,560.00
    Settlement s = settle(dl, corp);
    bool ok = s.total == 1080000 && s.covered == 756000 && s.patientPays == 324000;
    std::cout << "Dialysis + Corporate: total " << rs(s.total) << " covered " << rs(s.covered) << " patient " << rs(s.patientPays) << (ok ? "  PASS" : "  FAIL") << '\n'; failures += !ok;
    SelfPay self; Settlement s2 = settle(dl, self); ok = s2.patientPays == 1080000; std::cout << "Dialysis + Self: patient " << rs(s2.patientPays) << (ok ? "  PASS" : "  FAIL") << '\n'; failures += !ok;
    OutPatientBill op("Ravi", 50000, 120000); Settlement s3 = settle(op, corp); ok = s3.covered == 119000;       // 70% of 1,700.00
    std::cout << "OP + Corporate (old bill, new payer): covered " << rs(s3.covered) << (ok ? "  PASS" : "  FAIL") << '\n'; failures += !ok;
    InPatientBill ip("Meena", 3, 250000, 4000000, 150000);                                                        // 70% of 49,000.00 = 34,300.00 > budget
    try { settle(ip, corp); std::cout << "IP + Corporate: no exception  FAIL\n"; ++failures; }
    catch (const CoverExceededError& e) { std::cout << "IP + Corporate rejected: " << e.what() << "  PASS\n"; }

    std::cout << "\n=== 3. batch with errors: nothing is lost, nothing leaks ===\n";
    std::vector<std::unique_ptr<Bill>> bills; std::vector<std::unique_ptr<Payer>> payers; std::vector<std::string> unsettled;
    bills.push_back(std::make_unique<OutPatientBill>("Ravi", 50000, 120000));       payers.push_back(std::make_unique<Insurance>(100000, 0));   // claim above cover
    bills.push_back(std::make_unique<DayCareBill>("Kiran", "DC-XYZ", 100000));      payers.push_back(std::make_unique<GovernmentScheme>(std::map<std::string, Paise>{{"DC-CATARACT", 2500000}})); // unknown package
    bills.push_back(std::make_unique<DialysisBill>("Hari", 4, 250000, 80000));      payers.push_back(std::make_unique<CorporatePayer>(70, 2000000)); // ok
    bills.push_back(std::make_unique<EmergencyBill>("Asha", 100000, 4, 50000));     payers.push_back(std::make_unique<SelfPay>());                   // ok
    Paise collected = 0; int settled = 0;
    for (size_t i = 0; i < bills.size(); ++i) {
        try { Settlement r = settleLogged(*bills[i], *payers[i]); collected += r.total; ++settled; std::cout << "  settled " << bills[i]->who() << " via " << payers[i]->name() << ": " << rs(r.total) << '\n'; }
        catch (const ClaimError& e) { unsettled.push_back(bills[i]->who() + ": " + e.what()); }
        catch (const BillingError& e) { unsettled.push_back(bills[i]->who() + ": " + e.what()); }
        catch (...) { unsettled.push_back(bills[i]->who() + ": unknown error"); }
    }
    std::cout << "  settled " << settled << ", unsettled " << unsettled.size() << ", collected " << rs(collected) << '\n';
    for (auto& u : unsettled) std::cout << "  UNSETTLED -> " << u << '\n';
    ok = settled == 2 && unsettled.size() == 2 && settled + (int)unsettled.size() == (int)bills.size(); std::cout << (ok ? "no bill lost: PASS\n" : "BILL LOST: FAIL\n"); failures += !ok;

    std::cout << "\n=== 4. construction errors and a simulated allocation failure ===\n";
    try { OutPatientBill bad("X", -1, 0); } catch (const NegativeChargeError& e) { std::cout << "  NegativeChargeError: " << e.what() << '\n'; }
    try { Registry r; r.find("Zoya"); } catch (const MissingRecordError& e) { std::cout << "  MissingRecordError: " << e.what() << '\n'; }
    try { throw std::bad_alloc(); } catch (const std::bad_alloc& e) { std::cout << "  bad_alloc handled\n"; }
    std::cout << (failures == 0 ? "\nALL B4 CHECKS PASSED\n" : "\nSOME B4 CHECKS FAILED\n");
    return failures;
}
