//valgrind --leak-check=full -v -q --trace-children=yes --track-fds=yes ./executable
#include <iostream>
#include <math.h>
using namespace std;

class vector {
    int * _body;
    int _size;

public:
    vector (int sz, int b) {
        if (sz <= 0) cerr << "Badsize" << endl;
        else { 
            _body = new int [1 + (_size = sz)]; 
            for(int i=0; i<sz; i++) 
            _body[i] = b;
        }
    }

    int & operator[](int index) {
        if (_size < index || index < 0) cerr << "Badindex" << endl;
        return(_body[index]);
    }

    vector &operator= (const vector & rhs) {
        if (this == & rhs)
            return (*this);
        delete [] _body;
        _size = rhs._size;
        _body = new int [1 + _size];
        for (int i=0; i<_size; i++)
            _body[i] = rhs._body[i];
        return (*this);
    }

    vector &operator+ (const vector & op2) {
        if (_size != op2._size) cerr << "Badsize2" << endl;
        { 
            for(int i=0; i<_size; i++) { 
                _body[i]=op2._body[i]+_body[i]; //cout<<_body[i]; 
                }
            return(*this); 
        }
    }

    vector &operator- (const vector & op2) {
        if (_size != op2._size) cerr << "Badsize2" << endl;
        { 
            for(int i=0; i<_size; i++) { 
                _body[i]=op2._body[i]-_body[i]; //cout<<_body[i]; 
            }
            return(*this); 
        }
    }

    int getsize() {return(_size);}

    double scalar() {
        int a=0; 
        for(int i=0; i<_size; i++) 
            a+=(double)(_body[i]*_body[i]); 
        return(sqrt(a));
    }

    ~vector() {
        cout << "Destr ";
        delete []_body;
    }
};


int main()
{
    cout << "Создано 2 4-ёхмерных вектора, со значениями всех координат 1 и 3 соответственно" << endl;
    vector A(4, 1);   vector B(4, 3);   // Второй параметр - начальное значение координат
    cout << "Координата вектора: " << A[2] << endl;  // Индекс от 0 до 3 
    cout << "Длина вектора: " << A.getsize() << endl;
    cout << "Скалярное произведение A: " << A.scalar() << endl;
    cout << "Скалярное произведение B: " << B.scalar() << endl;
    A=A+B;
    cout << "Скалярное произведение (A+B): " << A.scalar() << endl;
    A=A-B;
    cout << "Скалярное произведение (A+B)-B: " << A.scalar() << endl;
    return(0);
}