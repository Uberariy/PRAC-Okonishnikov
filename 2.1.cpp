#include <iostream>
#include <math.h>
using namespace std;

class Cls {
    int i;
    Cls* operator= (const Cls);
public:
    Cls() { i = 1; }
};

void f(Cls * p, Cls * q) {
    *p = *q;
}

int main()
{
    return 0;
}

