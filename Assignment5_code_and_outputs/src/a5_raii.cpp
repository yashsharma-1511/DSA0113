// A5 alternative: delete in every catch block vs RAII (unique_ptr / vector)
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
struct BillData { std::vector<int> lines = std::vector<int>(100, 1); };
void validateStep(int step) { if (step == 3) throw std::runtime_error("bad claim"); }
#ifdef MANUAL
void processBill(int step) {
    BillData* b = new BillData;
    try {
        validateStep(step);          // may throw
    } catch (const std::runtime_error&) { delete b; throw; }   // delete #1
    // a NEW error path added later: this throw was added without a matching delete
    if (step == 4) throw std::logic_error("new error path");   // leak: no delete here
    delete b;                                                  // delete #2 (normal path)
}
#else
void processBill(int step) {
    auto b = std::make_unique<BillData>();
    validateStep(step);
    if (step == 4) throw std::logic_error("new error path");   // nothing to remember: b cleans itself
}
#endif
int main() {
    for (int s = 1; s <= 4; ++s) { try { processBill(s); std::cout << "step " << s << " ok\n"; }
        catch (const std::exception& e) { std::cout << "step " << s << " failed: " << e.what() << '\n'; } }
}
