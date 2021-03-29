#include <cmath>
#include <cstring>
#include <iostream>
using namespace std;

class Body {
    double destiny_, volume_;

public:

    Body(double des) : destiny_(des) {}
    virtual void Volume() = 0;
    double Mass() const {return (destiny_ * volume_);}
    double getdestiny() const {return destiny_;}
    void SetVol(double vol) {volume_ = vol;}
    friend std::ostream& operator<< (std::ostream &out, const Body &A);
};

class Cube : public Body {
    double len_;

public:

    Cube(double len, double des) : len_(len), Body(des) {}
    void Volume() {this->SetVol(len_*len_*len_);}
};

std::ostream& operator<< (std::ostream &out, const Body &A)
{
    cout << "Destiny: " << A.getdestiny() << " Mass: " << A.Mass() << "\n";
    return(out);
}

int main() 
{
    Cube A(2, 3);
    A.Volume();
    cout << A;
    return 0;
}