//valgrind --leak-check=full -v -q --trace-children=yes --track-fds=yes ./executable
#include <iostream>
#include <math.h>
#include <cstring>
#include <typeinfo>
using namespace std;

class BadRange {};
class SpecialInts {
    int *_A;
    int _size, _bad;

public:
    SpecialInts(int size=0) 
    {
        if (size < 0) cerr << "Badsize\n";
        _A = new int [size+1];
        _size = size;
    }

    ~SpecialInts()
    {
        cout << "Destr ";
        delete [] _A;
    }

    SpecialInts &operator, (const int & op2)
    {
        SpecialInts B(_size);
        for(int i=0; i<_size; i++) B._A[i] = _A[i];
        B._A[_size] = op2;
        _size++;
        delete [] _A;
        _A = new int [_size+1];
        for(int i=0; i<_size; i++) _A[i] = B._A[i];
        return(*this);
    }

    int &operator[] (const int & i)
    {
        if ((i < 0) || (i >= _size)) { _bad = i; throw BadRange(); }
        return(_A[i]);
    }

    int Get()
    {
        return(_bad);
    }

    int Len()
    {
        return(_size);
    }
};

int main()
{
    SpecialInts I;
    I,1,2,3;
    cout << I[0] << I[2] << "Size: " << I.Len();
    return(0);
}