#include <iostream>
#include <math.h>
#include <cstring>
#include <typeinfo>
using namespace std;

class BadSegment {};
class BadAddition {
    float _A1, _A2, _B1, _B2;

public:
    BadAddition(float A1, float B1, float A2, float B2) : _A1(A1), _A2(A2), _B1(B1), _B2(B2) {}

    float GetA1() const {return _A1;} 
    float GetA2() const {return _A2;}
    float GetB1() const {return _B1;}
    float GetB2() const {return _B2;}
};
class FloatPair {
private:
    float _a, _b;
public:
    FloatPair(float a, float b)
    {
        if (b < a) throw BadSegment();
        else {_a = a; _b = b;}
    }
    virtual float Measure() const = 0;

    float getA() const {return(_a);}
    float getB() const {return(_b);}
};

class Segment : FloatPair {
public:
    Segment(float a, float b) : FloatPair(a, b)
    {}

    float Measure() const
    {
        return(this->getB() - this->getA());
    }

    Segment operator+ (const Segment & S)
    {
        float a, b;
        if (this->getA() < S.getA())
        {
            if (this->getB() < S.getA()) throw BadAddition(this->getA(), this->getB(), S.getA(), S.getB());
        }
        else
        {
            if (this->getA() > S.getB()) throw BadAddition(this->getA(), this->getB(), S.getA(), S.getB());
        }
        b = (this->getB() < S.getB()) ? S.getB() : this->getB();
        a = (this->getA() > S.getA()) ? S.getA() : this->getA();
        Segment U(a, b);
        return(U);
    }
};

int main() 
{
    try {
        Segment f (1,2), g(0.5, 5), h(2.5, 6.5);
        std::cout << f.Measure() << std::endl;
        printf("%3.3f, %3.3f, %3.3f\n", (f+g).Measure(), (g+h).Measure(), (f+g+h).Measure());
        printf("%3.3f \n", (f+h).Measure());
    }
    catch (const BadAddition &bad) {
        printf("Bad addition: [%3.3f; %3.3f] + [%3.3f; %3.3f]\n", bad.GetA1(), bad.GetB1(), bad.GetA2(), bad.GetB2());
    }
    catch (BadSegment b) {
        printf("Bad segment\n");
    }
    return(0);
}