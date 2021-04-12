#include <cmath>
#include <cstring>
#include <iostream>
using namespace std;
//NOT FINISHED

class F {
    int x_;

public:
    int f;
    int Gett() {return(x_);}
    F operator() (int & x, int y) { x = x_ = y;}
};

class Assign {
    F *obj_;
    int x_;

public:
    Assign() { obj_ = new F; }
    ~Assign() { if (obj_) delete obj_; }
    F* operator-> () { x_ = (*obj_).Gett(); }
    F& operator* () {}
    int Get() { return(x_); }
};

int main() 
{
    Assign a;
    a->f(x,y);
    return 0;
}