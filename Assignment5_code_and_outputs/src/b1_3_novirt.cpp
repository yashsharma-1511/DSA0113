#include <iostream>
#include <memory>
#include <vector>
#ifdef NOVIRT
#define V
#else
#define V virtual
#endif
struct Bill { V long long total() const { return 0; } virtual ~Bill() = default; };
struct OP : Bill { long long total() const { return 170000; } };
struct IP : Bill { long long total() const { return 4900000; } };
int main() {
    std::vector<std::unique_ptr<Bill>> v;
    v.push_back(std::make_unique<OP>()); v.push_back(std::make_unique<IP>());
    long long s = 0; for (auto& b : v) { std::cout << b->total() << ' '; s += b->total(); }
    std::cout << "| day total " << s << '\n';
}
