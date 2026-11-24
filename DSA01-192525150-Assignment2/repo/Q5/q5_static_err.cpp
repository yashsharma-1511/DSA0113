class BusTicket {
    int seat = 0;
public:
    static int bad() { return seat; }
    static int bad2() { return this->seat; }
};
int main() { return BusTicket::bad(); }
