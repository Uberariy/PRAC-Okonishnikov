//valgrind --leak-check=full -v -q --trace-children=yes --track-fds=yes ./executable
#include <iostream>
#include <math.h>
using namespace std;

class A {
    int a;
public:
    A(int b=7) {a=b;}
    int operator*= (int c) {return a=a*c;}
    int get() {return a;}
};


int main () {
    A a1(5), a2 = 4, a3;
    a2 *= a1 *= 3;
    cout << a1.get( ) << ' ' << a2.get() << ' ' << a3.get( ) << endl;
    return 0;
}


