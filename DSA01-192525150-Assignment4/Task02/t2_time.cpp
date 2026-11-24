#include <chrono>
#include <iostream>
#include <string>
struct Person { std::string name; int id; };
struct StudentV : virtual Person { double cgpa; };
struct StaffV   : virtual Person { double salary; };
struct TaV : StudentV, StaffV {};
struct StudentN : Person { double cgpa; };
struct TaC { StudentN student; double salary; };
int main()
{
    const int N = 1000; TaV* v = new TaV[N]; TaC* c = new TaC[N];
    for (int i = 0; i < N; ++i) { v[i].id = i; c[i].student.id = i; }
    const int REP = 200000; volatile long long sink = 0; long long s1 = 0, s2 = 0;
    auto t0 = std::chrono::steady_clock::now();
    for (int r = 0; r < REP; ++r) for (int i = 0; i < N; ++i) s1 += v[i].id;           // through the virtual base
    auto t1 = std::chrono::steady_clock::now();
    for (int r = 0; r < REP; ++r) for (int i = 0; i < N; ++i) s2 += c[i].student.id;   // through a member
    auto t2 = std::chrono::steady_clock::now();
    sink = s1 + s2; (void)sink;
    std::cout << "200 million reads: virtual base " << std::chrono::duration<double, std::milli>(t1 - t0).count() << " ms, member " << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms\n";
    delete[] v; delete[] c;
}
