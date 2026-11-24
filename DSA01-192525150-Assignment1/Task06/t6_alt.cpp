#include <iostream>
#include <chrono>
enum class Zone { Inner, Middle, Outer };
constexpr long long FARE[] = {1000, 2000, 3000};
long long viaSwitch(Zone z){ switch(z){case Zone::Inner:return 1000;case Zone::Middle:return 2000;case Zone::Outer:return 3000;} return -1; }
long long viaArray(Zone z){ return FARE[static_cast<int>(z)]; }
int main(){
 const long long N=300000000; long long a=0,b=0;
 auto t0=std::chrono::steady_clock::now();
 for(long long i=0;i<N;i++) a+=viaSwitch(static_cast<Zone>(i%3));
 auto t1=std::chrono::steady_clock::now();
 for(long long i=0;i<N;i++) b+=viaArray(static_cast<Zone>(i%3));
 auto t2=std::chrono::steady_clock::now();
 std::cout<<"switch "<<std::chrono::duration<double,std::milli>(t1-t0).count()<<" ms, array "<<std::chrono::duration<double,std::milli>(t2-t1).count()<<" ms, sums "<<a<<" "<<b<<" sizeof(FARE)="<<sizeof(FARE)<<'\n';
}
