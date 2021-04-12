//valgrind --leak-check=full -v -q --trace-children=yes --track-fds=yes ./executable
#include <iostream>
#include <math.h>
#include <cstring>
#include <typeinfo>
using namespace std;

struct A { virtual void z () {} };
struct B: A { int x; B (int n = 7) { x = n; } };
B * f (void * p);
int main () {
    B b, * pb = f (& b);
    cout << pb -> x << endl;
    return 0;
}

B*& f (void *& point)
{
    A * c = static_cast <A *> (point);
    B * a = dynamic_cast <B *> (c);
    if (!a) exit(1);
    return(a);
}