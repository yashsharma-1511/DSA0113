#include <iostream>
#include <string>
class Address {
    std::string city_;
public:
    explicit Address(const std::string& c) : city_(c) {}
    const std::string& city() const { return city_; }
};
class Fellowship {
    std::string agency_; long long monthly_;
public:
    Fellowship(const std::string& a, long long m) : agency_(a), monthly_(m) {}
    long long monthly() const { return monthly_; }
};
class Person {                                   // root of the hierarchy
protected:
    std::string name_; int id_; std::string phone_;
    Address address_;                            // has-a (member class)
public:
    Person(const std::string& n, int i, const std::string& p, const std::string& city) : name_(n), id_(i), phone_(p), address_(city) {}
    virtual ~Person() = default;
    const std::string& name() const { return name_; }
};
class Student : public virtual Person {          // is-a Person
protected:
    std::string programme_; double cgpa_;
public:
    Student(const std::string& n, int i, const std::string& p, const std::string& c, const std::string& prog, double g)
        : Person(n, i, p, c), programme_(prog), cgpa_(g) {}
};
class Staff : public virtual Person {            // is-a Person
protected:
    std::string department_; double salary_;
public:
    Staff(const std::string& n, int i, const std::string& p, const std::string& c, const std::string& d, double s)
        : Person(n, i, p, c), department_(d), salary_(s) {}
};
class TeachingAssistant : public Student, public Staff {   // is-a Student and is-a Staff
public:
    TeachingAssistant(const std::string& n, int i, const std::string& p, const std::string& c, const std::string& prog, double g, const std::string& d, double s)
        : Person(n, i, p, c), Student(n, i, p, c, prog, g), Staff(n, i, p, c, d, s) {}
};
class ResearchScholar : public Student {         // multilevel: is-a Student, which is-a Person
    Fellowship fellowship_;                      // has-a Fellowship
public:
    ResearchScholar(const std::string& n, int i, const std::string& p, const std::string& c, const std::string& prog, double g, const Fellowship& f)
        : Person(n, i, p, c), Student(n, i, p, c, prog, g), fellowship_(f) {}
    long long fellowship() const { return fellowship_.monthly(); }
};
int main()
{
    TeachingAssistant ta("Kavya", 7, "98400", "Salem", "MTech", 8.9, "CSE", 25000);
    ResearchScholar rs("Arun", 8, "98401", "Chennai", "PhD", 9.1, Fellowship("UGC", 31000));
    std::cout << ta.name() << " (one name, no ambiguity), " << rs.name() << " fellowship " << rs.fellowship() << '\n';
    std::cout << "sizeof: Person " << sizeof(Person) << ", Student " << sizeof(Student) << ", Staff " << sizeof(Staff) << ", TeachingAssistant " << sizeof(TeachingAssistant) << ", ResearchScholar " << sizeof(ResearchScholar) << '\n';
}
