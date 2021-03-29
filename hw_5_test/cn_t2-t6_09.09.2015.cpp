#include <cstring>
#include <iostream>
#include <cmath>
using namespace std;

class Flag {
    bool obj;
public:
    Flag() {}
    void SetOn() { obj = 1; }
    void SetOff() { obj = 0; }
    bool IsOn() const { return obj; }
    operator bool () const { return obj; }
};

class Watched {
public:
    static int Amount;
    Watched() {Amount++;}
    Watched(const Watched & A) {Amount++;}
    ~Watched() {Amount--;}
};

int Watched::Amount = 0;

class A {
public:
    int operator[] (A a) { return 75; }
    int operator() () { return 50; }
    int operator() (A a) { return 100; }
    int operator() (A a, A aa) { return 150; }
    int operator() (A a, A aa, A aaa) { throw A(); return 0; }
    int operator-- () { return 500; }
    int operator-- (int) { return 501; }
};

class B {
public:
    operator A () { A a; return(a); }
    int operator[] (A a) { return 75; }
    int operator() (A a) { return 200; }
};

struct s1 {
    int f;
};

class D {
    s1 *obj;
public:
    D(s1 *ptr = 0) : obj(ptr) {}
    s1& operator* () const { return *obj; }
    s1* operator-> () const { return obj; }
    ~D() { if(obj) delete obj; }
};

int main()
{
    Flag J;
    J.SetOn();
    if (J) cout << "true" << "\n";;

    Watched I;
    cout << Watched::Amount << "\n";

    A a;    B b;
    cout << a[b] << b[a] << "\n";
    cout << a() << a(b) << a(b,b) << b(a) << "\n";
    //a(a,a,a);
    cout << --a << a-- << "\n";

    D d;
    d = new s1;
    d->f = 123;
    cout << d->f << "\n";

    return 0;
}
