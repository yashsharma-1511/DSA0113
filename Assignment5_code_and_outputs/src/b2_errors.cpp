#include "hospital.h"
#include <cstdlib>
#include <iostream>
#include <new>
#include <cmath>
struct Tracker { const char* n; explicit Tracker(const char* s) : n(s) { std::cout << "  [+" << n << "]\n"; } ~Tracker() { std::cout << "  [-" << n << "]\n"; } };
Paise parseRupees(const std::string& s) {
    size_t pos = 0;
    double v = std::stod(s, &pos);                    // throws std::invalid_argument if no digits
    if (pos != s.size()) throw std::invalid_argument("trailing text after number: " + s.substr(pos));
    return std::llround(v * 100);
}
#ifdef SMALLBIG
const unsigned long long BIG = 1ULL << 36;           // 512 GB: below the ASan allocator cap, above RAM
#else
const unsigned long long BIG = 1ULL << 59;
#endif
static int tests = 0, caughtOk = 0;
template <class F> void expectThrow(const char* label, F f) {
    ++tests;
    std::cout << "Test " << tests << ": " << label << '\n';
    try { Tracker t("guard"); f(); std::cout << "  NO EXCEPTION (unexpected)\n"; }
    catch (const CoverExceededError& e) { std::cout << "  caught CoverExceededError: " << e.what() << '\n'; ++caughtOk; }
    catch (const UnknownPackageError& e){ std::cout << "  caught UnknownPackageError: " << e.what() << '\n'; ++caughtOk; }
    catch (const NegativeChargeError& e){ std::cout << "  caught NegativeChargeError: " << e.what() << '\n'; ++caughtOk; }
    catch (const MissingRecordError& e) { std::cout << "  caught MissingRecordError: " << e.what() << '\n'; ++caughtOk; }
    catch (const std::invalid_argument& e){ std::cout << "  caught std::invalid_argument: " << e.what() << '\n'; ++caughtOk; }
    catch (const std::bad_alloc& e)     { std::cout << "  caught std::bad_alloc: " << e.what() << '\n'; ++caughtOk; }
}
int main() {
    expectThrow("negative consultation charge", [] { OutPatientBill b("Ravi", -50000, 1000); });
    expectThrow("claim above cover (limit 1,000.00, bill 1,700.00)", [] { OutPatientBill b("Ravi", 50000, 120000); Insurance i(100000, 0); i.amountCovered(b); });
    expectThrow("unknown package code DC-XYZ", [] { DayCareBill b("Kiran", "DC-XYZ", 100000); GovernmentScheme g({{"DC-CATARACT", 2500000}}); g.amountCovered(b); });
    expectThrow("missing patient record", [] { Registry r; r.add("Ravi", 1); r.find("Zoya"); });
    expectThrow("text mixed into a number: 12O0.50 (letter O)", [] { parseRupees("12O0.50"); });
    expectThrow("text with no digits at all", [] { parseRupees("abc"); });
    expectThrow("memory allocation failure (new double[huge])", [] { double* p = new double[BIG]; p[0] = 1; volatile double d = p[0]; (void)d; delete[] p; });
    std::cout << "\nstod(\"12.5abc\") = " << std::stod("12.5abc") << "  (plain stod silently ignores trailing text; parseRupees checks pos)\n";
    std::cout << "\n--- top-level catch(...) keeps the program running ---\n";
    for (int i = 1; i <= 3; ++i) {
        try {
            Tracker t("bill-session");
            if (i == 2) throw 7;                      // a non-std exception
            std::cout << "  bill " << i << " processed\n";
        } catch (const std::exception& e) { std::cout << "  std error " << e.what() << '\n'; }
        catch (...) { std::cout << "  unknown error caught at top level; continuing\n"; }
    }
    std::cout << "\n" << caughtOk << " of " << tests << " error tests handled; program still running\n";
    return caughtOk == tests ? 0 : 1;
}
