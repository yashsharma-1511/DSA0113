#include <iostream>
#include <stdexcept>
struct Closer { ~Closer() { std::cout << "closing ledger\n"; throw std::runtime_error("close failed"); } };
int main() {
    try { Closer c; std::cout << "working\n"; }
    catch (const std::exception& e) { std::cout << "caught " << e.what() << '\n'; }
    std::cout << "after\n";
}
