#include <chrono>
#include <fstream>
#include <iostream>
int main()
{
    std::ofstream("fares.cfg") << "1000 2000 3000\n";
    auto t0 = std::chrono::steady_clock::now();
    long long fare[3] = {0, 0, 0};
    std::ifstream in("fares.cfg");
    for (int i = 0; i < 3; ++i) in >> fare[i];
    auto t1 = std::chrono::steady_clock::now();
    bool good = in.good() || in.eof();
    std::cout << "read " << fare[0] << ' ' << fare[1] << ' ' << fare[2] << " ok=" << std::boolalpha << good << " in "
              << std::chrono::duration<double, std::micro>(t1 - t0).count() << " us; sizeof(fare)=" << sizeof(fare) << " bytes\n";
    std::ofstream("bad.cfg") << "1000 abc 3000\n";
    std::ifstream bad("bad.cfg"); long long x[3] = {0, 0, 0};
    for (int i = 0; i < 3; ++i) bad >> x[i];
    std::cout << "bad file: fail=" << bad.fail() << " values " << x[0] << ' ' << x[1] << ' ' << x[2] << "  (must be rejected at start-up)\n";
}
