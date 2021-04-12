#include <cmath>
#include <cstring>
#include <iostream>
using namespace std;

class M {
    int *obj_;
    int num_;
    static int count;

public:

    M(int n) {obj_ = new int;   *obj_ = n; count++;   num_=count;}
    M() {obj_ = new int;    count++;   num_=count;}
    int& operator* () const { return(*obj_); }
    int& operator() (M & m) {
        M A(0);
        if (num_ == m.num_)
        return(*obj_);
        else 
        return(*A);
    }
    ~M() { delete obj_; }

};

int M::count = 0;

int main() 
{
    M m(10), x(20);
    cout << m(m) << m(x) << x(m) << x(x);
    m(m) = 30;
    cout << m(m) << m(x);
    return 0;
}