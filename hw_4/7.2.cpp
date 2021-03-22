//valgrind --leak-check=full -v -q --trace-children=yes --track-fds=yes ./executable
#include <iostream>
#include <math.h>
#include <cstring>
#include <typeinfo>
using namespace std;

struct B { virtual void g () {} };
struct D: B { char y [100]; };
void f9 (B & b, D & d, int n) {
D * pd = (n > 0) ? & d : (D *) & b;
    if (typeid(*pd) == typeid(D))
    strcpy (pd -> y, "one_variant\n");
}
