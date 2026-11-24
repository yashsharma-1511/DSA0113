#include <string>
class Professor { public: double pay() { return 1; } };
class LabAssistant{ public: double pay() { return 2; } };
int main()
{
    Professor p; LabAssistant l;
    auto* all[] = { &p, &l };            // one loop over every staff member?
    double total = 0;
    for (auto* s : all) total += s->pay();
    return static_cast<int>(total);
}
