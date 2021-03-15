//valgrind --leak-check=full -v -q --trace-children=yes --track-fds=yes ./executable
#include <iostream>
#include <math.h>
using namespace std;

struct S {
S ( int a) {
 try { if (a > 0) throw *this;
 else 
 if (a < 0) throw 0; 
}
catch ( S & ) { 
 cout << "SCatch_S&" << endl; }
 catch (int) { throw; }
 cout << "SConstr" << endl;
}
S (const S & a) { cout << "Copy" << endl; }
~S ( ) { cout << "Destr" << endl; }
};
int main ( ) { 
 try { S s1( 0 ); S s2 ( 5 );
cout << "Main" << endl; 
 }
catch (S &) { cout << "MainCatch_S&" << endl; }
catch ( ... ) { cout << "MainCatch_..." << endl; }
return 0;
}

/*
SConstr
Copy
SCatch_S&
Destr
SConstr
Main
Destr
Destr
*/