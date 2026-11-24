#include <iostream>
#include <string>
class Address {
    std::string city_;
public:
    explicit Address(const std::string& c) : city_(c) { std::cout << "Address(" << city_ << ") "; }
    ~Address() { std::cout << "~Address "; }
};
class Course {
    std::string code_;
public:
    explicit Course(const std::string& c) : code_(c) { std::cout << "Course(" << code_ << ") "; }
    ~Course() { std::cout << "~Course "; }
};
class Person {
protected:
    std::string name_; int id_; Address address_;                 // member class
public:
    Person(const std::string& n, int i, const std::string& city) : name_(n), id_(i), address_(city) { std::cout << "Person "; }
    virtual ~Person() { std::cout << "~Person "; }
};
class Student : public virtual Person {
protected:
    double cgpa_; Course course_;                                 // member class
public:
    Student(const std::string& n, int i, const std::string& city, double g, const std::string& course)
        : Person(n, i, city), cgpa_(g), course_(course) { std::cout << "Student "; }
    ~Student() { std::cout << "~Student "; }
};
class Staff : public virtual Person {
protected:
    double salary_;
public:
    Staff(const std::string& n, int i, const std::string& city, double s) : Person(n, i, city), salary_(s) { std::cout << "Staff "; }
    ~Staff() { std::cout << "~Staff "; }
};
class TeachingAssistant : public Student, public Staff {
public:
    TeachingAssistant(const std::string& n, int i, const std::string& city, double g, const std::string& course, double s)
        : Person(n, i, city), Student(n, i, city, g, course), Staff(n, i, city, s) { std::cout << "TA "; }
    ~TeachingAssistant() { std::cout << "~TA "; }
};
int main()
{
    std::cout << "Student: ";           { Student s("Arun", 1, "Salem", 8.5, "CS101"); }                          std::cout << '\n';
    std::cout << "Staff:   ";           { Staff f("Meena", 2, "Erode", 40000); }                                  std::cout << '\n';
    std::cout << "TA:      ";           { TeachingAssistant t("Kavya", 3, "Salem", 8.9, "CS101", 25000); }        std::cout << '\n';
}
