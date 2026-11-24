// Space copied and time per call: value / reference / const reference for long long and a large Student
#include <chrono>
#include <iostream>
#include <cstring>
struct Student { char name[64]; char address[200]; long long fee; int marks[20]; };
__attribute__((noinline)) long long feeVal(Student s, int p) { return s.fee - s.fee * p / 100; }
__attribute__((noinline)) long long feeRef(Student& s, int p) { return s.fee - s.fee * p / 100; }
__attribute__((noinline)) long long feeCRef(const Student& s, int p) { return s.fee - s.fee * p / 100; }
__attribute__((noinline)) long long llVal(long long f, int p) { return f - f * p / 100; }
__attribute__((noinline)) long long llRef(const long long& f, int p) { return f - f * p / 100; }
template <class F> double timeit(F f)
{
    double best = 1e18;
    for (int r = 0; r < 5; ++r) {
        auto t0 = std::chrono::steady_clock::now();
        f();
        auto t1 = std::chrono::steady_clock::now();
        double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        if (ms < best) best = ms;
    }
    return best;
}
int main()
{
    Student s{}; std::strcpy(s.name, "Yash"); s.fee = 45000;
    long long fee = 45000;
    const int N = 10000000;
    volatile long long sink = 0;
    std::cout << "sizeof(long long)=" << sizeof(long long) << " sizeof(Student)=" << sizeof(Student)
              << " sizeof(Student&)~sizeof(pointer)=" << sizeof(void*) << '\n';
    std::cout << "long long by value        : " << timeit([&]{ long long a=0; for (int i=0;i<N;++i) a+=llVal(fee, 10 + (i&3)); sink=a; }) << " ms\n";
    std::cout << "long long by const ref    : " << timeit([&]{ long long a=0; for (int i=0;i<N;++i) a+=llRef(fee, 10 + (i&3)); sink=a; }) << " ms\n";
    std::cout << "Student by value          : " << timeit([&]{ long long a=0; for (int i=0;i<N;++i) a+=feeVal(s, 10 + (i&3)); sink=a; }) << " ms\n";
    std::cout << "Student by reference      : " << timeit([&]{ long long a=0; for (int i=0;i<N;++i) a+=feeRef(s, 10 + (i&3)); sink=a; }) << " ms\n";
    std::cout << "Student by const reference: " << timeit([&]{ long long a=0; for (int i=0;i<N;++i) a+=feeCRef(s, 10 + (i&3)); sink=a; }) << " ms\n";
}
