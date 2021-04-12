#include <cmath>
#include <cstring>
#include <iostream>
using namespace std;

class SumHold {
    int sumacc, sumrej;
    static int allsum;

public:
    SumHold() {sumacc = 0;  sumrej = 0;}
    SumHold& operator<< (int i) 
    {
        if (Acceptable(i)) sumacc += i; 
        else sumrej += i;
        allsum += i; 
        return(*this);
    }
    int Get() const {return(sumacc);}
    int GetRejected() const {return(sumrej);}
    int GetAll() const {return(allsum);}
    void Write() const { cout << sumacc << "\t" << sumrej << "\n";}
    void WriteAll() const { cout << allsum << "\n";}
    virtual bool Acceptable(int i) { return(true); }

};

int SumHold::allsum = 0;

class SumHoldSmall : public SumHold {

    bool Acceptable(int i) override { if ((i <= 10) && (i >= -10)) return(true); else return(false); }
};

int main() 
{
    SumHoldSmall sh, sh1;
    sh<<10<<30<<40<<50;
    sh1<<10<<-5<<40;
    sh.Write();
    sh1.Write();
    sh.WriteAll();
    return 0;
}