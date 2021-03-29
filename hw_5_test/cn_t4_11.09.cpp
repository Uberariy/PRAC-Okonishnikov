#include <cmath>
#include <cstring>
#include <iostream>
using namespace std;

class SumHold {
    int sum;

public:
    SumHold() {sum = 0;}
    SumHold& operator<< (int i) {sum += i;  return(*this);}
    int Get() const {return(sum);}
    void Write() const { cout << "\t" << sum << "\n";}

};


int main() 
{
    SumHold sh;
    sh<<20<<30<<40<<50;
    sh.Write();
    return 0;
}