double charge(int grams, bool express = false, bool insured = false) { return grams; }
double charge(double kg, bool express = false, bool insured = false) { return kg; }
int main() { return (int)charge(2L); }   // long: int or double?  both are conversions
