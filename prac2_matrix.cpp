#include <iostream>
#include <cmath>
#include <cstring>
using namespace std;

class Matrix {
protected:
    int **_A;
    int _len, _wid;

public:
    static int countcallF, countMatrix;  // Static value, represents amount of calls to funtion F
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
            countMatrix++;
        }
    }

    ~Matrix() 
    {
        int i, j;
        for (i=0; i<_len; i++) delete[] _A[i];
        delete[] _A;
        countMatrix--;
    }

    Matrix (const Matrix & A)
    {
        int i, j;
        _len = A._len; _wid = A._wid;
        _A = (int **) new int* [_len];
        for (i=0; i<_len; i++)
            _A[i] = (int *) new int [_wid];     
        for (j=0; j<_wid; j++)   for (i=0; i<_len; i++)   _A[i][j] = A._A[i][j];  
        countMatrix++;      
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
    
    virtual int F () = 0;
};

class RegMatrix: public Matrix {
public:
    RegMatrix (int len, int wid, int start=0): Matrix (len, wid, start) {}

    RegMatrix operator+ (const RegMatrix & A)
    {
        int i, j;
        if ((_wid != A._wid) || (_len != A._len))  { cerr << "+: wrong indexes\n"; exit(1); } 
        else
        {
            RegMatrix B(_len, _wid, 0);
            for (j=0; j<_wid; j++)   for (i=0; i<_len; i++)   
            { 
                B._A[i][j] += A._A[i][j];
                B._A[i][j] += _A[i][j];
            }
            return(B);
        }
    }

    RegMatrix operator- (const RegMatrix & A)
    {
        int i, j;
        if ((_wid != A._wid) || (_len != A._len))  { cerr << "-: wrong indexes\n"; exit(1); } 
        else 
        {
            RegMatrix B(_len, _wid, 0);
            for (j=0; j<_wid; j++)   for (i=0; i<_len; i++)   
            { 
                B._A[i][j] -= A._A[i][j];
                B._A[i][j] += _A[i][j];
            }
            return(B);
        }
    } 

    RegMatrix operator* (int n)
    {
        int i, j;
        RegMatrix B(_len, _wid, 0);
        for (i=0; i<_len; i++)
            for (j=0; j<_wid; j++) 
                B._A[i][j] = _A[i][j] * n;
        return(B);
    }

    RegMatrix operator* (const RegMatrix & A)   // For regular matrixes, * is just a common multiplication operation
    {
        int i, j, k;
        if (_wid != A._len)  { cerr << "*: wrong indexes\n"; exit(1); }
        else 
        {
            RegMatrix B(_len, A._wid, 0);
            for (i=0; i<_len; i++)
                for (j=0; j<A._wid; j++)
                    for (k=0; k<_wid; k++)
                        { B._A[i][j] += _A[i][k] * A._A[k][j];}
            return(B);
        }
    }  

    int F ()
    {
        int i, j, sum=0;
        for (j=0; j<_wid; j++)   for (i=0; i<_len; i++)   
            sum += _A[i][j];
        countcallF++;
        return(sum);
    }
};

class Scalar: public Matrix {
public:
    Scalar (int len, int start=0): Matrix (len, 1, start) {}

    Scalar operator+ (const Scalar & A)
    {
        int i;
        if (_len != A._len)  { cerr << "+: wrong indexes\n"; exit(1); } 
        else
        {
            Scalar B(_len, 0);
            for (i=0; i<_len; i++)   
            { 
                B._A[i][0] += A._A[i][0];
                B._A[i][0] += _A[i][0];
            }
            return(B);
        }   
    }

    Scalar operator- (const Scalar & A)
    {
        int i;
        if (_len != A._len)  { cerr << "-: wrong indexes\n"; exit(1); } 
        else
        {
            Scalar B(_len, 0);
            for (i=0; i<_len; i++)   
            { 
                B._A[i][0] -= A._A[i][0];
                B._A[i][0] += _A[i][0];
            }
            return(B);
        }   
    }

    int operator* (const Scalar & A)
    {
        int i, sum;
        if (_len != A._len)  { cerr << "*: wrong size\n"; exit(1); }
        else 
        {
            sum = 0;
            for (i=0; i<_len; i++)
                { sum += _A[i][0] * A._A[i][0];}
            return(sum);
        }
    } 

    int F () 
    {    
        countcallF++;
        return((*this)*(*this));   
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

int Matrix::countcallF = 0;
int Matrix::countMatrix = 0;

int main()
{
    RegMatrix A(3, 2, 1);   A(0, 1) = A(1, 0) = A(2, 1) = 2;
    RegMatrix B(4, 3, 2);   B(1, 0) = B(0, 1) = B(1, 2) = 3;
    RegMatrix C(1, 1, 1);

    cout << "Матрицы А и B:\n" << A << B;
    C = B * A;
    cout << "Произведение двух матриц - матрица С:\n" << C;

    RegMatrix D(2, 2, 4);
    cout << "Матрица D:\n" << D;

    cout << "Произведение матрицы A на число 0:\n" << A*0;
    cout << "Произведение матрицы A на число 10:\n" << A*10;
    
    Scalar I(4, 4);
    Scalar J(4, 1);

    cout << "Применение функции F для RegMatrix - Сумма всех элементов матрицы C:\n" << C.F() << "\n";
    cout << "Применение функции F для Scalar - Скалярный квадрат суммы I + J:\n" << (J + I).F() << "\n";
    cout << "Количество вызовов функции F (статическая переменная countcallF):\n" << Matrix::countcallF << "\n";
    cout << "Количество созданных матриц (обоих типов) (статическая переменная countMatrix):\n" << Matrix::countMatrix << "\n";
    return(0);
}