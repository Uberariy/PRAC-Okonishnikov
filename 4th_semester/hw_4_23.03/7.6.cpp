//valgrind --leak-check=full -v -q --trace-children=yes --track-fds=yes ./executable
#include <iostream>
#include <math.h>
#include <cstring>
#include <typeinfo>
using namespace std;

struct A { virtual void z () {} };
struct B: A { int x; B (int y = 5) { x = y; } };
B * f (A * pa);
int main (){
    try { 
        B b, * pb = f (& b);
        cout << pb -> x << endl;
        A a; pb = f (& a);
        cout << pb -> x << endl;
    }
    catch (...) { }
    return 0;
}

B* f(A * point)
{
    B * pd = dynamic_cast <B *> (point);
    if (!pd) exit(1);
    return (pd);
}