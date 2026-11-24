#include <iostream>
#include <string>
using std::string;
class Professor { public: string name; int id; double basic; double da; double research; double pay() { return basic + basic * 0.42 + research; } };
class LabAssistant{ public: string name; int id; double basic; double da; double pay() { return basic + basic * 0.42; } };
class GuestFaculty{ public: string nm; int code; double hours; double rate; double pay() { return hours * rate; } };
int main()
{
    Professor p{"Meena", 101, 80000, 0, 5000};
    LabAssistant l{"Ravi", 201, 30000, 0};
    GuestFaculty g{"Anitha", 301, 20, 1500};
    p.basic = -5;                                   // public data: nothing stops a bad value
    std::cout << p.pay() << ' ' << l.pay() << ' ' << g.pay() << '\n';
    std::cout << "sizeof: " << sizeof(Professor) << ' ' << sizeof(LabAssistant) << ' ' << sizeof(GuestFaculty) << '\n';
}
