#include <iostream>
#include <climits>
#include <cstdio>
#include <iomanip>
int main(){
 std::cout<<"bool "<<sizeof(bool)<<" char "<<sizeof(char)<<" int "<<sizeof(int)<<" unsigned "<<sizeof(unsigned)<<" long long "<<sizeof(long long)<<" float "<<sizeof(float)<<" double "<<sizeof(double)<<'\n';
 std::cout<<"INT_MAX "<<INT_MAX<<" UINT_MAX "<<UINT_MAX<<" LLONG_MAX "<<LLONG_MAX<<" SHRT_MAX "<<SHRT_MAX<<" CHAR_MAX "<<CHAR_MAX<<'\n';
 std::cout<<std::setprecision(17);
 std::cout<<"0.1+0.2 = "<<0.1+0.2<<'\n';
 std::cout<<"1.1*3 = "<<1.1*3<<'\n';
 double s=0; for(int i=0;i<10;i++) s+=0.10;
 std::cout<<"ten times 0.10 = "<<s<<" ==1.00 ? "<<std::boolalpha<<(s==1.0)<<'\n';
 std::cout<<"0.1*3 == 0.3 ? "<<(0.1*3==0.3)<<'\n';
 std::cout<<"true+2 = "<<true+2<<"  7/2.0 = "<<7/2.0<<"  -1<1u ? "<<(-1<1u)<<'\n';
}
