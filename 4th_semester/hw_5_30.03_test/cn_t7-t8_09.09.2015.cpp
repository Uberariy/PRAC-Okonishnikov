#include <cmath>
#include <cstring>
#include <iostream>
using namespace std;
// NOT FINISHED!!!

class QueueEmpty {};
class IntQueue {
    struct list
    {
        int num;
        struct list *next;
    } *L_;
    struct list *end_;
    int size_;

public:

    void FreeQueue (struct list *L)
    {
        if (L!=NULL)
        {
            FreeQueue(L -> next);
            delete L;
        }
    }
    IntQueue()
    {   
        L_ = NULL;
        size_ = 0;    
    }
    ~IntQueue()
    {
        FreeQueue(L_);
    }
    IntQueue& operator<<= (int A) 
    {
        size_++;
        list *tmp = new list;
        tmp -> num = A;
        tmp -> next = NULL;
        if (L_ == NULL) { L_ = tmp; end_ = L_ -> next; }
        end_ -> next = tmp;
        return(*this);
    }    
    void operator>>= (int & A) 
    {
        if (!size_) throw QueueEmpty();
        size_--;
        A = L_ -> num;
        struct list *tmp_ = L_ -> next;
        delete L_;
        L_ = tmp_;
    }
    int Amount () {return size_;}
};

int main() 
{   
    IntQueue Q;
    Q<<=10; Q<<=20; Q<<=30;
    cout << Q.Amount() << "\n";
    int a, b;  Q>>=a;  Q>>=b;
    cout << a << "\n" << b << "\n" << Q.Amount() << "\n";
    return 0;
}