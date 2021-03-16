#include <iostream>
#include <cmath>
#include <cstring>
using namespace std;

class M {
    int **_A;
    int _len, _wid;

public:
    friend std::ostream& operator<< (std::ostream &out, const M &A);

    M (int len, int wid, int st=0)
    {
        int i, j;
        if ((len < 0) || (wid < 0)) { cerr << "CONSTR: wrong size\n"; exit(1); }
        else 
            _A = (int **) new int* [len];
            for (i; i<len; i++)
                _A[i] = (int *) new int [wid];
            for (j=0; j<wid; j++)   for (i; i<len; i++)   _A[i][j] = st;
            _len = len; _wid = wid;
    }

    ~M() 
    {
        int i, j;
        for (i; i<_len; i++) delete[] _A[i];
        delete[] _A;
    }

//    int*& M::operator[] (const int i)
  //  {   // returns i line
    //    return _A[i];
   // }

    int& operator() (int i, int j)
    {
       if ((i < 0) || (j < 0) || (i > _len) || (j > _wid)) { cerr << "(): wrong indexes\n"; } 
       else return(_A[i][j]);
    }

    M (const M & A)
    {
        int i, j;
        if ((_wid != A._wid) || (_len != A._len))  { cerr << "ConstrC: wrong indexes\n"; exit(1); } 
        else for (j=0; j<_wid; j++)   for (i=0; i<_len; i++)   _A[i][j] = A._A[i][j];
    }

    M operator+ (const M & A)
    {
        int i, j;
        if ((_wid != A._wid) || (_len != A._len))  { cerr << "+: wrong indexes\n"; exit(1); } 
        else for (j=0; j<_wid; j++)   for (i=0; i<_len; i++)   _A[i][j] += A._A[i][j];
    }

    M operator- (const M & A)
    {
        int i, j;
        if ((_wid != A._wid) || (_len != A._len))  { cerr << "-: wrong indexes\n"; exit(1); } 
        else for (j=0; j<_wid; j++)   for (i=0; i<_len; i++)   _A[i][j] -= A._A[i][j];
    } 

    M operator* (const M & A)
    {
        int i, j;
        if ((_wid != A._len) || (_len != A._wid))  { cerr << "*: wrong indexes\n"; exit(1); }
        else 
        {
            M B(_len, _len, 0);
            for (j=0; j<A._len; j++)
                for (i=0; i<A._wid; i++)
                    B._A[i][j] += _A[i][j] * A._A[j][i];
            // Тут остановился
        }
    }  
};

std::ostream& operator<< (std::ostream &out, const M &A)
{
    int i, j;
    for (i=0; i<A._len; i++)
        for (j=0; j<A._wid; j++);
}