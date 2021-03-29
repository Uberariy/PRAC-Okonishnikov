#include <cmath>
#include <cstring>
#include <iostream>
using namespace std;
// NOT FINISHED!!!

class IntQueue {
    struct list
    {
        int num;
        struct list *next;
    } *L;
    struct list *start;
    struct list *end;
    int size;

public:

    struct list *listadd (struct list *L, int A)
    {
        if (L == NULL) 
        {
            L = (list *) new list;
            L -> num = A;
            end = L;
        }
        else L -> next  = listadd(L -> next, A);
    }

    void operator<<= (int A) 
    {
        L = listadd(L, A);
    }
    int operator>>= (int & A) 
    {

    }
    int Amount () {return size;}
};

int main() 
{
    return 0;
}