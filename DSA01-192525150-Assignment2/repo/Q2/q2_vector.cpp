#include <iostream>
#include <vector>
int main()
{
    std::vector<int> marks = {56, 71, 88};
    std::cout << "capacity before = " << marks.capacity() << '\n';
    int& top = marks[2];
    int* before = &marks[0];
    marks.push_back(60);                 // capacity 3 is full -> reallocation
    std::cout << "capacity after  = " << marks.capacity() << '\n';
    std::cout << "storage moved? " << std::boolalpha << (before != &marks[0]) << '\n';
    std::cout << "reference still aliases marks[2]? " << (&top == &marks[2]) << '\n';
    top += 2;                            // undefined behaviour: writes to freed memory
    std::cout << "marks[2] = " << marks[2] << " (expected 90)\n";
}
