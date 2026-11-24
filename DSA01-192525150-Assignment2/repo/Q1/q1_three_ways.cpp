#include <iostream>
void byValue(long long fee, int percent, const long long* callerAddr)
{
    fee -= fee * percent / 100;
    std::cout << "  inside by value   : local fee = " << fee
              << ", same object as caller? " << std::boolalpha << (&fee == callerAddr) << '\n';
}
void byReference(long long& fee, int percent, const long long* callerAddr)
{
    fee -= fee * percent / 100;
    std::cout << "  inside by reference: fee = " << fee
              << ", same object as caller? " << (&fee == callerAddr) << '\n';
}
void byPointer(long long* fee, int percent, const long long* callerAddr)
{
    *fee -= *fee * percent / 100;
    std::cout << "  inside by pointer : *fee = " << *fee
              << ", pointer holds caller's address? " << (fee == callerAddr)
              << ", pointer itself is the caller's object? " << (reinterpret_cast<const long long*>(&fee) == callerAddr) << '\n';
}
int main()
{
    long long fee = 45000;
    std::cout << "start: " << fee << '\n';
    byValue(fee, 10, &fee);
    std::cout << "stored fee after by value (merit 10%)     : " << fee << '\n';
    byReference(fee, 10, &fee);
    std::cout << "stored fee after by reference (merit 10%) : " << fee << '\n';
    byPointer(&fee, 15, &fee);
    std::cout << "stored fee after by pointer (sports 15%)  : " << fee << '\n';
}
