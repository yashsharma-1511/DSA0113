#include <iostream>
int main()
{
 int a{7}, b{2};
 double c{static_cast<double>(a) / b};
 char ch{static_cast<char>('A' + 2)};
 bool flag{a != 0};
 unsigned int u{1};
 int s{-1};
 std::cout << c << ' ' << ch << ' ' << static_cast<int>(ch) + 1 << ' ' << std::boolalpha << flag << '\n';
 std::cout << (static_cast<long long>(s) < static_cast<long long>(u) ? "s < u" : "s >= u") << '\n';
}
