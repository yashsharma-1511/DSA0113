struct Address { int pin; };
class StudentShallow {
    Address* home;
public:
    explicit StudentShallow(int pin) : home(new Address{pin}) {}
    ~StudentShallow() { delete home; }                // compiler-made copy shares this pointer
};
int main()
{
    StudentShallow a(636001);
    StudentShallow b = a;                             // default copy: two owners of one Address
}
