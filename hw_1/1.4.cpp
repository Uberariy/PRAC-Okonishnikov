//valgrind --leak-check=full -v -q --trace-children=yes --track-fds=yes ./executable
#include <iostream>
#include <math.h>
using namespace std;

class A {
    int a;
public:
    A(int b) { a=b;}
    int operator*= (int b) {return(a = a*b);}
    int get() {return a;}
};


int main () {
    A a1 (5), a2 = 3;
    a1 *= 10;
    a2 *= a1 *= 2;
    cout << a1.get() << a2.get() << endl;
    return 0;
}
