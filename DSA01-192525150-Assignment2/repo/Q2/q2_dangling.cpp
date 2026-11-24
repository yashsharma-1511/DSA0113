#include <iostream>
int& bad()
{
    int local = 42;
    return local;          // reference to a variable that dies at the closing brace
}
int main()
{
    int& r = bad();
    std::cout << "value through dangling reference: " << r << '\n';
    int other = 7;
    std::cout << "other = " << other << '\n';
}
