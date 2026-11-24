#include <iostream>
#include <chrono>
constexpr bool isPeakExpr(int h){ return (h >= 8 && h < 10) || (h >= 17 && h < 20); }
int main(){
 bool peakTable[24];
 for (int h = 0; h < 24; ++h) peakTable[h] = isPeakExpr(h);
 bool ok = true; for (int h = 0; h < 24; ++h) if (peakTable[h] != isPeakExpr(h)) ok = false;
 std::cout << "tables agree: " << std::boolalpha << ok << "  peak hours:";
 for (int h = 0; h < 24; ++h) if (peakTable[h]) std::cout << ' ' << h;
 std::cout << "\nhour 10 -> " << peakTable[10] << ", hour 20 -> " << peakTable[20] << ", hour 8 -> " << peakTable[8] << ", hour 17 -> " << peakTable[17] << '\n';
 const long long N = 200000000; volatile int sink;
 auto t0 = std::chrono::steady_clock::now(); long long c1 = 0;
 for (long long i = 0; i < N; ++i) c1 += isPeakExpr(static_cast<int>(i % 24));
 auto t1 = std::chrono::steady_clock::now(); long long c2 = 0;
 for (long long i = 0; i < N; ++i) c2 += peakTable[static_cast<int>(i % 24)];
 auto t2 = std::chrono::steady_clock::now(); sink = static_cast<int>(c1 + c2);
 std::cout << "expression " << std::chrono::duration<double, std::milli>(t1 - t0).count() << " ms, table " << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms, counts " << c1 << " " << c2 << '\n';
}
