#include <iostream>
int main(int argc, char** argv)
{
    int unusedCount = 5;
    unsigned int u = 1; int s = -1;
    if (s < u) std::cout << "less\n";
    int arr[3] = {1, 2, 3};
    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i) std::cout << arr[i];
    int x; std::cout << x << '\n';
    double d = 3.9; int k = d;
    return k;
}
