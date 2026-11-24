// Student-derived design: static counters, waiting list, array larger than capacity, tickets inside a function
#include <iostream>
#include <string>
class BusTicket {
    static int issued;               // seats given out (belongs to the whole class)
    static int waiting;              // tickets on the waiting list
    static const int CAPACITY = 3;   // one place to change the bus size
    int seat;                        // 0 = no seat
    int waitNo;                      // position on the waiting list, 0 = not waiting
    std::string name;
public:
    explicit BusTicket(const std::string& n) : seat(0), waitNo(0), name(n)
    {
        if (issued < CAPACITY) seat = ++issued;
        else waitNo = ++waiting;
    }
    static int count() { return issued; }
    static int waitingCount() { return waiting; }
    // static int bad() { return seat; }   // error: a static member function has no 'this'
    void print() const
    {
        std::cout << name << ": ";
        if (seat) std::cout << "Seat " << seat; else std::cout << "Waiting list #" << waitNo;
        std::cout << '\n';
    }
};
int BusTicket::issued = 0;
int BusTicket::waiting = 0;
int main()
{
    std::cout << "-- array of 6 tickets, capacity 3 --\n";
    BusTicket trip[6] = {BusTicket("Asha"), BusTicket("Bala"), BusTicket("Chitra"),
                         BusTicket("Dev"), BusTicket("Esha"), BusTicket("Farid")};
    for (const BusTicket& t : trip) t.print();
    std::cout << "Issued: " << BusTicket::count() << "  Waiting: " << BusTicket::waitingCount() << '\n';
}
