// Time to add a transaction: shifting array vs circular buffer vs std::deque
#include <chrono>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <new>
#ifndef HISTORY
#define HISTORY 10
#endif
static size_t g_bytes = 0, g_allocs = 0;
void* operator new(size_t n) { g_bytes += n; ++g_allocs; void* p = std::malloc(n); if (!p) throw std::bad_alloc(); return p; }
void operator delete(void* p) noexcept { std::free(p); }
void operator delete(void* p, size_t) noexcept { std::free(p); }
struct Transaction { int no; char type; long long amount; };
const int K = HISTORY;
struct Shift { Transaction t[K]; int count = 0, total = 0;
    void add(char ty, long long a) { if (count == K) { for (int i = 0; i < K - 1; ++i) t[i] = t[i + 1]; --count; } t[count++] = {++total, ty, a}; }
    long long sum() const { long long s = 0; for (int i = 0; i < count; ++i) s += t[i].amount; return s; } };
struct Ring { Transaction t[K]; int start = 0, count = 0, total = 0;
    void add(char ty, long long a) { if (count == K) { t[start] = {++total, ty, a}; start = (start + 1) % K; } else { t[(start + count) % K] = {++total, ty, a}; ++count; } }
    long long sum() const { long long s = 0; for (int i = 0; i < count; ++i) s += t[(start + i) % K].amount; return s; } };
struct Deq { std::deque<Transaction> d; int total = 0;
    void add(char ty, long long a) { if ((int)d.size() == K) d.pop_front(); d.push_back({++total, ty, a}); }
    long long sum() const { long long s = 0; for (auto& x : d) s += x.amount; return s; } };
template <class A> double run(long long N, long long& chk)
{
    double best = 1e18;
    for (int r = 0; r < 3; ++r) {
        A acc; auto t0 = std::chrono::steady_clock::now();
        for (long long i = 0; i < N; ++i) acc.add(i & 1 ? 'W' : 'D', i % 1000);
        auto t1 = std::chrono::steady_clock::now();
        chk = acc.sum();
        double ns = std::chrono::duration<double, std::nano>(t1 - t0).count() / N;
        if (ns < best) best = ns;
    }
    return best;
}
int main(int argc, char** argv)
{
    long long N = argc > 1 ? std::atoll(argv[1]) : 2000000;
    long long c1, c2, c3;
    double a = run<Shift>(N, c1), b = run<Ring>(N, c2);
    g_bytes = g_allocs = 0; double c = run<Deq>(N, c3);
    std::cout << "K=" << K << " N=" << N << "  shift " << a << " ns/add  ring " << b << " ns/add  deque " << c << " ns/add\n";
    std::cout << "   checksum of last K amounts equal: " << std::boolalpha << (c1 == c2 && c2 == c3) << " (" << c1 << ")\n";
    std::cout << "   sizeof: shift " << sizeof(Shift) << "  ring " << sizeof(Ring) << "  deque object " << sizeof(Deq) << '\n';
    { g_bytes = g_allocs = 0; Deq d; for (int i = 0; i < K; ++i) d.add('D', i); std::cout << "   deque heap for " << K << " entries: " << g_bytes << " bytes in " << g_allocs << " allocations\n"; }
}
