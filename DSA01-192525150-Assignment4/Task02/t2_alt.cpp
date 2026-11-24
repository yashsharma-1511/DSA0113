#include <iostream>
#include <string>
struct Person { std::string name; int id; std::string phone; };
struct StudentV : virtual Person { std::string programme; double cgpa; };
struct StaffV   : virtual Person { std::string department; double salary; };
struct TaV : StudentV, StaffV {};                 // virtual diamond: one Person
struct StudentN : Person { std::string programme; double cgpa; };
struct StaffN   : Person { std::string department; double salary; };
struct TaN : StudentN, StaffN {};                 // non-virtual: two Person sub-objects
struct TaC { StudentN student; StaffN staff; };   // composition: two members, two Persons
int main()
{
    std::cout << "sizeof(Person)=" << sizeof(Person) << " Student(virtual)=" << sizeof(StudentV) << " Student(plain)=" << sizeof(StudentN) << '\n';
    std::cout << "TA virtual diamond : " << sizeof(TaV) << " bytes (one Person)\n";
    std::cout << "TA non-virtual     : " << sizeof(TaN) << " bytes (two Person)\n";
    std::cout << "TA composition     : " << sizeof(TaC) << " bytes (two Person)\n";
    TaV a; a.name = "Kavya";                      // one name, reached directly
    TaC c; c.student.name = "Kavya"; c.staff.name = "Kavya";   // two names to keep equal
    std::cout << "virtual: " << a.name << " | composition: " << c.student.name << " and " << c.staff.name << '\n';
}
