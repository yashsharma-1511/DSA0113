#include <chrono>
#include <iostream>
#include <string>
class Address {
    std::string city; int pin;
public:
    Address(const std::string& c, int p) : city(c), pin(p) {}
    std::string str() const { return city + "-" + std::to_string(pin); }
};
class Person { protected: std::string name; int age; public: Person(const std::string& n, int a) : name(n), age(a) {} };
class StudentPtr : public Person {
    long long regNo;
    Address* home;                                              // dynamically created member
public:
    StudentPtr(const std::string& n, int a, long long reg, const std::string& city, int pin)
        : Person(n, a), regNo(reg), home(new Address(city, pin)) {}          // constructor allocates
    StudentPtr(const StudentPtr& o) : Person(o), regNo(o.regNo), home(new Address(*o.home)) {}   // deep copy
    StudentPtr& operator=(const StudentPtr& o) { if (this != &o) { Person::operator=(o); regNo = o.regNo; *home = *o.home; } return *this; }
    ~StudentPtr() { delete home; }                                          // destructor releases
    void print() const { std::cout << regNo << ' ' << name << " (" << age << ") " << home->str() << '\n'; }
};
class StudentShallow : public Person {                           // same, but compiler-made copy
    Address* home;
public:
    StudentShallow(const std::string& n, int a, const std::string& city, int pin) : Person(n, a), home(new Address(city, pin)) {}
    ~StudentShallow() { delete home; }
};
class StudentObj : public Person { long long regNo; Address home; public: StudentObj(const std::string& n, int a, long long r, const std::string& c, int p) : Person(n, a), regNo(r), home(c, p) {} };
int main(int argc, char**)
{
    StudentPtr s("Kavya", 19, 192311001, "Salem", 636001);
    StudentPtr t = s;                                          // copy constructor
    t.print();
    std::cout << "sizeof member version " << sizeof(StudentObj) << ", pointer version " << sizeof(StudentPtr) << " (+ " << sizeof(Address) << " bytes on the heap)\n";
    const int N = 2000000; volatile long long sink = 0;
    auto t0 = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i) { StudentObj o("Kavya", 19, i, "Salem", 636001); sink = sink + sizeof(o); }
    auto t1 = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i) { StudentPtr o("Kavya", 19, i, "Salem", 636001); sink = sink + sizeof(o); }
    auto t2 = std::chrono::steady_clock::now();
    std::cout << "2 million constructions: member " << std::chrono::duration<double, std::milli>(t1 - t0).count() << " ms, pointer " << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms\n";
    if (argc > 1) { StudentShallow a("A", 1, "Salem", 1); StudentShallow b = a; }          // default copy: two owners of one Address
}
