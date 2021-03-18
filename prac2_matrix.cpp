#include <iostream>
#include <cmath>
#include <cstring>
using namespace std;

class Matrix {
    int **_A;
    int _len, _wid;

public:
    friend std::ostream& operator<< (std::ostream &out, const Matrix &A);

    Matrix (int len, int wid, int start=0)  // "start" represent initial value for matrix elements
    {
        int i, j;        
        if ((len <= 0) || (wid <= 0)) { cerr << "CONSTR: wrong size\n"; exit(1); }
        else 
        {
            _A = (int **) new int* [len];
            for (i=0; i<len; i++)
                _A[i] = (int *) new int [wid];
            for (j=0; j<wid; j++)   for (i=0; i<len; i++)   _A[i][j] = start;
            _len = len; _wid = wid;
        }
    }

    ~Matrix() 
    {
        int i, j;
        for (i=0; i<_len; i++) delete[] _A[i];
        delete[] _A;
    }

    Matrix (const Matrix & A)
    {
        int i, j;
        _len = A._len; _wid = A._wid;
        _A = (int **) new int* [_len];
        for (i=0; i<_len; i++)
            _A[i] = (int *) new int [_wid];     
        for (j=0; j<_wid; j++)   for (i=0; i<_len; i++)   _A[i][j] = A._A[i][j];        
    }

    int& operator() (int i, int j) const
    {
       if ((i < 0) || (j < 0) || (i > _len) || (j > _wid)) { cerr << "(): wrong indexes\n"; exit(1);} 
       else return(_A[i][j]);
    }

    Matrix &operator= (const Matrix & A)
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

    Matrix operator+ (const Matrix & A)
    {
        int i, j;
        if ((_wid != A._wid) || (_len != A._len))  { cerr << "+: wrong indexes\n"; exit(1); } 
        else
        {
            Matrix B(_len, _wid, 0);
            for (j=0; j<_wid; j++)   for (i=0; i<_len; i++)   
            { 
                B._A[i][j] += A._A[i][j];
                B._A[i][j] += _A[i][j];
            }
            return(B);
        }
    }

    Matrix operator- (const Matrix & A)
    {
        int i, j;
        if ((_wid != A._wid) || (_len != A._len))  { cerr << "-: wrong indexes\n"; exit(1); } 
        else 
        {
            Matrix B(_len, _wid, 0);
            for (j=0; j<_wid; j++)   for (i=0; i<_len; i++)   
            { 
                B._A[i][j] -= A._A[i][j];
                B._A[i][j] += _A[i][j];
            }
            return(B);
        }
    } 

    Matrix operator* (const Matrix & A)
    {
        int i, j, k;
        if (_wid != A._len)  { cerr << "*: wrong indexes\n"; exit(1); }
        else 
        {
            Matrix B(_len, _len, 0);
            for (i=0; i<_len; i++)
                for (j=0; j<_len; j++)
                    for (k=0; k<_wid; k++)
                        { B._A[i][j] += _A[i][k] * A._A[k][i];}
            return(B);
        }
    }  

    Matrix operator* (int n)
    {
        int i, j;
        Matrix B(_len, _wid, 0);
        for (i=0; i<_len; i++)
            for (j=0; j<_wid; j++) 
                B._A[i][j] = _A[i][j] * n;
        return(B);
    }
};

std::ostream& operator<< (std::ostream &out, const Matrix &A)
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
    Matrix A(3, 2, 1);   A(0, 1) = A(1, 0) = A(2, 1) = 2;
    Matrix B(2, 3, 2);   B(1, 0) = B(0, 1) = B(1, 2) = 3;
    Matrix C(1, 1, 1);

    cout << "Матрицы А и B:\n" << A << B;
    C = B * A;
    cout << "Произведение двух матриц - матрица С:\n" << C;

    Matrix D(2, 2, 4);
    cout << "Матрица D:\n" << D;
    cout << "Сумма двух матриц С и D:\n" << C+D;

    cout << "Произведение матрицы A на число 0:\n" << A*0;
    cout << "Произведение матрицы A на число 10:\n" << A*10;
    return(0);
}