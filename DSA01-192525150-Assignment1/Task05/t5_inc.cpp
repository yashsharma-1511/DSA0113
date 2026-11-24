#include <iostream>
int main(){
 int i = 5; i = i++;
 int a = 5; int p = a++; int q = 5; int r = ++q;
 std::cout << "i = i++ gives " << i << "; a++ gives " << p << " (a=" << a << "); ++q gives " << r << " (q=" << q << ")\n";
 int j = 1; int m = j++ + j++;
 std::cout << "j++ + j++ : m = " << m << ", j = " << j << '\n';
}
