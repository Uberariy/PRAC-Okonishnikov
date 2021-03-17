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
        if ((len <= 0) || (wid <= 0)) { cerr << "CONSTR: wrong size\n"; exit(1); }
        else 
        {
            _A = (int **) new int* [len];
            for (i=0; i<len; i++)
                _A[i] = (int *) new int [wid];
            for (j=0; j<wid; j++)   for (i=0; i<len; i++)   _A[i][j] = st;
            _len = len; _wid = wid;
        }
    }

    ~M() 
    {
        int i, j;
        for (i=0; i<_len; i++) delete[] _A[i];
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

    M &operator= (const M & A)
    {
        int i, j;
        for (i=0; i<_len; i++) delete[] _A[i];
        delete[] _A;
        _len = A._len; _wid = A._wid;
        _A = (int **) new int* [_len];
        for (i=0; i<_len; i++)
            _A[i] = (int *) new int [_wid];     
        for (j=0; j<_wid; j++)   for (i=0; i<_len; i++)   _A[i][j] = A._A[i][j];
        return(*this);
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

    M &operator* (const M & A)
    {
        int i, j, k;
        if ((_wid != A._len) || (_len != A._wid))  { cerr << "*: wrong indexes\n"; exit(1); }
        else 
        {
            M B(_len, _len, 0); //cout << _len;
            for (i=0; i<_len; i++)
                for (j=0; j<_len; j++)
                    for (k=0; k<_wid; k++)
                        { B._A[i][j] += _A[i][k] * A._A[k][i];}// cout << B._A[i][j] << " ";}

            for (i=0; i<_len; i++) delete[] _A[i];
            delete[] _A;
            _wid = _len;
            _A = (int **) new int* [_len];
            for (i=0; i<_len; i++)
                _A[i] = (int *) new int [_wid];            

            for (j=0; j<_len; j++)   for (i=0; i<_len; i++)   {_A[i][j] = B._A[i][j]; }//fprintf(stderr, "a");}
            return(*this);
        }
    }  
};

std::ostream& operator<< (std::ostream &out, const M &A)
{
    int i, j;
    for (i=0; i<A._len; i++)
    {
        for (j=0; j<A._wid; j++)    out << A._A[i][j] << "\t";
        out << "\n";
    }
    out << "\n";    return(out);
}

int main()
{
    M A(3, 2, 1);
    M B(2, 3, 2);
    M C(1, 1, 1);
    cout << A << B;
    A = A*B;
    cout << A;
    return(0);
}