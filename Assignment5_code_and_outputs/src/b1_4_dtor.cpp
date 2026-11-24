#include <iostream>
#include <string>
#include <vector>
#ifdef NOVDTOR
#define VD
#else
#define VD virtual
#endif
struct Bill { std::string patient; explicit Bill(std::string p) : patient(std::move(p)) {} VD ~Bill() {} virtual long long total() const = 0; };
struct InPatientBill : Bill {
    std::vector<long long> lineItems;        // heap memory owned by the derived class
    std::string ward;
    InPatientBill(std::string p) : Bill(std::move(p)), lineItems(1000, 100), ward(100, 'W') {}
    long long total() const override { long long s = 0; for (auto x : lineItems) s += x; return s; }
};
int main() {
    Bill* b = new InPatientBill("Meena");
    std::cout << b->total() << '\n';
    delete b;
}
