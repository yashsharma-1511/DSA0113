#include <iostream>
#include <chrono>
#include <cmath>
#include <iomanip>
int main(){
 long long ns[]={10,1000,100000,1000000,3000000,10000000};
 std::cout<<std::scientific<<std::setprecision(3);
 for(long long n:ns){
  double a=0,b=0; long long p=0,q=0;
  for(long long i=0;i<n;i++){a+=10.80;b+=0.10;p+=1080;q+=10;}
  std::cout<<n<<"  "<<a-p/100.0<<"  "<<b-q/100.0<<'\n';
 }
 const long long N=100000000;
 volatile long long vs=0; volatile double vd=0;
 auto t0=std::chrono::steady_clock::now();
 long long s=0; for(long long i=0;i<N;i++) s+=i%7+1080; vs=s;
 auto t1=std::chrono::steady_clock::now();
 double d=0; for(long long i=0;i<N;i++) d+=(double)(i%7)+1080.0; vd=d;
 auto t2=std::chrono::steady_clock::now();
 std::cout<<std::fixed<<std::setprecision(0)<<"ll "<<std::chrono::duration<double,std::milli>(t1-t0).count()<<" ms, double "<<std::chrono::duration<double,std::milli>(t2-t1).count()<<" ms\n";
}
