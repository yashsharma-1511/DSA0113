#include <string>
struct Person { std::string name; };
struct Student : Person { double cgpa; };
struct Staff : Person { double salary; };
struct TeachingAssistant : Student, Staff {};
int main() { TeachingAssistant ta; ta.name = "Kavya"; }
