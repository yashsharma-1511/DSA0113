#include <iostream>
double charge(double weight)                                  { return 40 * weight; }
double charge(double weight, bool express)                    { return 40 * weight * (express ? 1.5 : 1.0); }
double charge(double weight, bool express = false, bool insured = false) { return 40 * weight * (express ? 1.5 : 1.0) + (insured ? 25 : 0); }
int main()
{
    std::cout << charge(2.5) << '\n';          // charge(double)  or  charge(double, bool=false, bool=false) ?
    std::cout << charge(2.5, true) << '\n';    // charge(double, bool)  or  the 3-parameter one ?
    std::cout << charge(2.5, true, true) << '\n'; // only the third matches
}
