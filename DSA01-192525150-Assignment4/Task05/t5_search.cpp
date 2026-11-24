#include <iomanip>
#include <iostream>
#include <string>
class Cart {
    double sum_ = 0; std::string owner_;
public:
    Cart(const std::string& o, double s) : sum_(s), owner_(o) {}
    double total() const { return sum_; }
    const std::string& owner() const { return owner_; }
};
int main()
{
    Cart* carts[5] = { new Cart("Arun", 324.0), new Cart("Meena", 810.5), new Cart("Ravi", 55.0), new Cart("Kavya", 810.5), new Cart("Anitha", 120.0) };
    std::cout << std::fixed << std::setprecision(2);
    int best = 0;                                                  // indexing
    for (int i = 1; i < 5; ++i) if (carts[i]->total() > carts[best]->total()) best = i;
    std::cout << "by indexing        : " << carts[best]->owner() << " " << carts[best]->total() << '\n';
    Cart** top = carts;                                            // pointer arithmetic
    for (Cart** it = carts + 1; it != carts + 5; ++it) if ((*it)->total() > (*top)->total()) top = it;
    std::cout << "by pointer arithm. : " << (*top)->owner() << " " << (*top)->total() << " (position " << top - carts << ")\n";
    for (Cart* c : carts) delete c;
}
