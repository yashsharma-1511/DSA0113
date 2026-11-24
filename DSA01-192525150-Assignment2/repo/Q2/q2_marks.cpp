#include <iostream>
const int N = 6;
int& highest(int marks[], int n)              // returns an alias to the element itself
{
    int best = 0;
    for (int i = 1; i < n; ++i) if (marks[i] > marks[best]) best = i;
    return marks[best];
}
const int& highestConst(const int marks[], int n) // alias, but read-only
{
    int best = 0;
    for (int i = 1; i < n; ++i) if (marks[i] > marks[best]) best = i;
    return marks[best];
}
int highestCopy(const int marks[], int n)     // returns a copy of the value
{
    int best = 0;
    for (int i = 1; i < n; ++i) if (marks[i] > marks[best]) best = i;
    return marks[best];
}
void print(const char* label, const int m[], int n)
{
    std::cout << label;
    for (int i = 0; i < n; ++i) std::cout << ' ' << m[i];
    std::cout << '\n';
}
int main()
{
    int marks[N] = {56, 71, 88, 64, 79, 45};
    print("original            :", marks, N);
    highest(marks, N) += 2;                        // moderation +2 applied to the array element
    print("after highest()+=2  :", marks, N);
    highest(marks, N) = 95;                        // call on the left of =
    print("after highest()=95  :", marks, N);
    int copy = highestCopy(marks, N); copy += 5;   // changes only the copy
    print("after copy += 5     :", marks, N);
    const int& cr = highestConst(marks, N);        // read-only alias
    std::cout << "const ref sees    : " << cr << '\n';
    marks[2] = 91;                                 // change the element; the alias sees it
    std::cout << "after marks[2]=91 : const ref " << cr << ", copy " << copy << '\n';
    // highestConst(marks, N) = 5;   // error: assignment of read-only location
}
