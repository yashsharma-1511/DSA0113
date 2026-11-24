class Person { public: int pub=1; protected: int prot=2; private: int priv=3; };
class Student : public Person {};
class Grand : public Student { public: int get() { return 0; } };
int main() {
  Student s; return s.pub; }
