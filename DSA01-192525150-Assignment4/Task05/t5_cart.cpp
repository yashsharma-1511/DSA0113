#include <iomanip>
#include <iostream>
#include <string>
class Cart {
    std::string items[10];
    double prices[10];
    int n = 0;
    double discount = 0;                                           // percent
public:
    Cart& add(const std::string& item, double price)               // returns *this: calls can be chained
    {
        if (n < 10) { items[n] = item; prices[n] = price; ++n; }
        return *this;
    }
    Cart& setDiscount(double discount)                             // parameter hides the member
    {
        this->discount = discount;                                 // this-> picks the member
        return *this;
    }
    double total() const
    {
        double t = 0;
        for (int i = 0; i < n; ++i) t += prices[i];
        return t * (100 - discount) / 100;
    }
    const Cart* self() const { return this; }                      // this is the address of the current object
};
void useByReference(Cart& r) { r.add("Tea", 90); }
int main()
{
    std::cout << std::fixed << std::setprecision(2);
    Cart a;                                                        // (a) directly by name
    a.add("Rice", 60).add("Dal", 120).setDiscount(10);
    std::cout << "(a) by name        : " << a.total() << '\n';
    Cart* p = &a;                                                  // (b) through a pointer
    p->add("Oil", 180);
    std::cout << "(b) pointer ->     : " << p->total() << '\n';
    useByReference(a);                                             // (c) through a reference
    std::cout << "(c) reference      : " << a.total() << '\n';
    Cart* q = new Cart;                                            // (d) created with new
    q->add("Milk", 50).add("Eggs", 70).setDiscount(5);
    std::cout << "(d) new / delete   : " << q->total() << '\n';
    std::cout << "this == &object    : " << std::boolalpha << (q->self() == q) << '\n';
    delete q;                                                      // (d) released with delete
}
