#include <iostream>
#include <vector>
int main()
{
    std::vector<int> marks = {56, 71, 88};
    marks.reserve(100);
    int& top = marks[2];
    for (int i = 0; i < 50; ++i) marks.push_back(i);   // stays below capacity: no reallocation
    top += 2;
    std::cout << "with reserve(100): marks[2] = " << marks[2] << ", still same element? " << std::boolalpha << (&top == &marks[2]) << '\n';
}
