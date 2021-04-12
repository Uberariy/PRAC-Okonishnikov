//valgrind --leak-check=full -v -q --trace-children=yes --track-fds=yes ./executable
#include <iostream>
#include <math.h>
using namespace std;

class B {
    int a=10;
public:
    B(B & b) { a=a+b.a; }
    B() {}
    int get() {return a;}
};


int main () {
    B b1, b2 = b1, b3 (b2);
    cout << b1.get() << b2.get() << b3.get() << endl;
    return 0;
}

