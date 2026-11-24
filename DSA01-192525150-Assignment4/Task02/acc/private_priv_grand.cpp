class Person { public: int pub=1; protected: int prot=2; private: int priv=3; };
class Student : private Person {};
class Grand : public Student { public: int get() { return priv; } };
int main() {
  Grand g; return g.get(); }
