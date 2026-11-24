// B3.3 + B2 alternative: return codes vs exceptions vs std::optional, 1,000,000 bills, -O2
#include <chrono>
#include <cstdio>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
using Clock = std::chrono::steady_clock;
class BillingError : public std::runtime_error { public: using std::runtime_error::runtime_error; };
class NegativeChargeError : public BillingError { public: using BillingError::BillingError; };
enum class Status { Ok, Negative };
Status validateRC(long long amount, long long& out) { if (amount < 0) return Status::Negative; out = amount; return Status::Ok; }
long long validateEx(long long amount) { if (amount < 0) throw NegativeChargeError("negative charge"); return amount; }
std::optional<long long> validateOpt(long long amount) { if (amount < 0) return std::nullopt; return amount; }
volatile long long sink;
int main() {
    const int N = 1000000; const int pcts[] = {0, 1, 50};
    std::printf("sizeof(Status)=%zu sizeof(optional<long long>)=%zu sizeof(long long)=%zu\n", sizeof(Status), sizeof(std::optional<long long>), sizeof(long long));
    for (int pct : pcts) {
        std::vector<long long> a(N); for (int i = 0; i < N; ++i) a[i] = ((static_cast<long long>(i) * 7919) % 100 < pct) ? -(i % 1000) - 1 : 1000 + i % 5000;
        double bestRC = 1e18, bestEx = 1e18, bestOpt = 1e18; long long sRC = 0, sEx = 0, sOpt = 0; long bad = 0;
        for (int rep = 0; rep < 3; ++rep) {
            auto t0 = Clock::now(); sRC = 0; bad = 0; for (int i = 0; i < N; ++i) { long long o; if (validateRC(a[i], o) == Status::Ok) sRC += o; else ++bad; } sink = sRC;
            auto t1 = Clock::now(); bestRC = std::min(bestRC, std::chrono::duration<double, std::milli>(t1 - t0).count());
            t0 = Clock::now(); sEx = 0; long bad2 = 0; for (int i = 0; i < N; ++i) { try { sEx += validateEx(a[i]); } catch (const BillingError&) { ++bad2; } } sink = sEx;
            t1 = Clock::now(); bestEx = std::min(bestEx, std::chrono::duration<double, std::milli>(t1 - t0).count());
            t0 = Clock::now(); sOpt = 0; long bad3 = 0; for (int i = 0; i < N; ++i) { auto r = validateOpt(a[i]); if (r) sOpt += *r; else ++bad3; } sink = sOpt;
            t1 = Clock::now(); bestOpt = std::min(bestOpt, std::chrono::duration<double, std::milli>(t1 - t0).count());
            if (bad != bad2 || bad != bad3 || sRC != sEx || sEx != sOpt) std::printf("MISMATCH\n");
        }
        std::printf("invalid=%2d%% (%ld bad)  return-code=%.2f ms  exceptions=%.2f ms  optional=%.2f ms  ratio exc/rc=%.1f\n", pct, bad, bestRC, bestEx, bestOpt, bestEx / bestRC);
        std::fflush(stdout);
    }
}
