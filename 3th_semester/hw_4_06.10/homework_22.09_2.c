#include <stdio.h>

void primes(int n) // Функция, печатающие все простые числа, меньшие n
{
    int i,j;
    for (j=1; j<n; j++)
    {
        for (i=2; (i<=j/2) || (printf("%d, \n", j)==0); i++)
            if (j%i==0) break;
    }        
}

int main()
{
    primes(30);
    return(1);
}
