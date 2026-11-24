#include <iostream>
#include <string>
#include <iomanip>
constexpr double DA_RATE = 0.42;                 // dearness allowance, written once

class Staff {                                    // abstract base: common data and the pay() contract
    std::string name_;
    int id_;
protected:
    double basic_;
    Staff(const std::string& name, int id, double basic) : name_(name), id_(id), basic_(basic) {}
    double basicWithDA() const { return basic_ + basic_ * DA_RATE; }
public:
    virtual ~Staff() = default;
    virtual double pay() const = 0;              // every category must define its own rule
    const std::string& name() const { return name_; }
    int id() const { return id_; }
};
class Professor : public Staff {
    double research_;
public:
    Professor(const std::string& n, int id, double basic, double research) : Staff(n, id, basic), research_(research) {}
    double pay() const override { return basicWithDA() + research_; }
};
class LabAssistant : public Staff {
public:
    LabAssistant(const std::string& n, int id, double basic) : Staff(n, id, basic) {}
    double pay() const override { return basicWithDA(); }
};
class GuestFaculty : public Staff {
    double hours_, rate_;
public:
    GuestFaculty(const std::string& n, int id, double hours, double rate) : Staff(n, id, 0), hours_(hours), rate_(rate) {}
    double pay() const override { return hours_ * rate_; }
};
int main()
{
    Professor p("Meena", 101, 80000, 5000);
    LabAssistant l("Ravi", 201, 30000);
    GuestFaculty g("Anitha", 301, 20, 1500);
    const Staff* all[] = { &p, &l, &g };         // one array, one loop
    double total = 0;
    std::cout << std::fixed << std::setprecision(2);
    for (const Staff* s : all) {
        std::cout << std::setw(8) << s->name() << ' ' << s->id() << "  pay " << s->pay() << '\n';
        total += s->pay();
    }
    std::cout << "total payroll " << total << '\n';
}
