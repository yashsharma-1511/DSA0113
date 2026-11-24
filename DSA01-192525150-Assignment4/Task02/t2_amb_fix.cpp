#include <string>
struct Person { std::string name; };
struct Student : virtual Person { double cgpa; };
struct Staff : virtual Person { double salary; };
struct TeachingAssistant : Student, Staff {};
int main() { TeachingAssistant ta; ta.name = "Kavya"; }
