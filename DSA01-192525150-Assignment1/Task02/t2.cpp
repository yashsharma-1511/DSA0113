#include <iostream>
#define FARE_INNER 500 + 500
constexpr int FARE_INNER_C = 500 + 500;
enum class Zone { Inner, Middle, Outer };
enum OldZone { OInner, OMiddle, OOuter };
int main(){
  std::cout<<"macro  FARE_INNER*2     = "<<FARE_INNER*2<<"  (expected 2000)\n";
  std::cout<<"constexpr FARE_INNER_C*2 = "<<FARE_INNER_C*2<<'\n';
  std::cout<<"sizeof(Zone)="<<sizeof(Zone)<<" sizeof(OldZone)="<<sizeof(OldZone)<<'\n';
  std::cout<<"plain enum arithmetic compiles: "<<OOuter+5<<'\n';
  std::cout<<"OInner == 0 ? "<<(OInner==0)<<'\n';
}
