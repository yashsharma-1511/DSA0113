#include <iostream>
#include <string>
struct Person { Person(int) { std::cout << "Person "; } ~Person() { std::cout << "~Person "; } long long data[4]; };
struct Student : Person { Student() : Person(1) { std::cout << "Student "; } ~Student() { std::cout << "~Student "; } };
struct Staff : Person { Staff() : Person(2) { std::cout << "Staff "; } ~Staff() { std::cout << "~Staff "; } };
struct TeachingAssistant : Student, Staff { TeachingAssistant() { std::cout << "TA "; } ~TeachingAssistant() { std::cout << "~TA "; } };
struct PersonV { PersonV(int) { std::cout << ""; } long long data[4]; };
struct StudentV : virtual PersonV { StudentV() : PersonV(1) {} };
struct StaffV : virtual PersonV { StaffV() : PersonV(2) {} };
struct TaV : StudentV, StaffV { TaV() : PersonV(3) {} };
int main()
{
    { TeachingAssistant t; std::cout << "| "; }
    std::cout << "\nsizeof(TeachingAssistant) non-virtual = " << sizeof(TeachingAssistant) << ", virtual version = " << sizeof(TaV) << ", sizeof(Person) = " << sizeof(Person) << '\n';
}
