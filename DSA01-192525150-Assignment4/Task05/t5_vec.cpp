#include <chrono>
#include <iostream>
#include <string>
#include <vector>
class Cart {
    std::string owner_; double sum_;
    std::string items[10]; double prices[10] = {};
public:
    Cart(const std::string& o, double s) : owner_(o), sum_(s) {}
    double total() const { return sum_; }
};
int main(int argc, char**)
{
    std::vector<Cart> v;
    v.reserve(2);
    v.emplace_back("Arun", 324.0);
    Cart* p = &v[0];                                               // pointer into the vector
    std::cout << "capacity " << v.capacity() << ", &v[0] = " << static_cast<const void*>(p) << '\n';
    v.emplace_back("Meena", 810.5);
    v.emplace_back("Ravi", 55.0);                                  // exceeds capacity: vector reallocates
    std::cout << "capacity " << v.capacity() << ", &v[0] = " << static_cast<const void*>(&v[0]) << ", old pointer equal? " << std::boolalpha << (p == &v[0]) << '\n';
    if (argc > 1) std::cout << "total through old pointer: " << p->total() << '\n';     // dangling
    std::cout << "sizeof(Cart) = " << sizeof(Cart) << ", sizeof(Cart*) = " << sizeof(Cart*) << '\n';
    if (argc > 2) return 0;
    const int N = 100000; std::vector<Cart> objs; std::vector<Cart*> ptrs;
    for (int i = 0; i < N; ++i) { objs.emplace_back("x", (i * 37) % 1000); }
    for (int i = 0; i < N; ++i) ptrs.push_back(new Cart("x", (i * 37) % 1000));
    volatile double sink = 0; const int REP = 200;
    auto t0 = std::chrono::steady_clock::now();
    double best1 = 0; for (int r = 0; r < REP; ++r) for (const Cart& c : objs) if (c.total() > best1) best1 = c.total();
    auto t1 = std::chrono::steady_clock::now();
    double best2 = 0; for (int r = 0; r < REP; ++r) for (const Cart* c : ptrs) if (c->total() > best2) best2 = c->total();
    auto t2 = std::chrono::steady_clock::now();
    sink = best1 + best2; (void)sink;
    std::cout << "find highest among 100000 carts x 200: vector<Cart> " << std::chrono::duration<double, std::milli>(t1 - t0).count() << " ms, Cart* array " << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms, same answer " << (best1 == best2) << '\n';
    std::cout << "memory: objects " << N * sizeof(Cart) / 1000000 << " MB in the vector; pointers " << N * sizeof(Cart*) / 1000 << " KB plus the carts themselves\n";
    for (Cart* c : ptrs) delete c;
}
