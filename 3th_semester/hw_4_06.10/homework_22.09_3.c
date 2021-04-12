#include <stdio.h>

void fibprimes(int n) // Функция, печатающая все простые числа Фибоначчи, меньшие n
{
  int v1=1, v2=1, tmp, i;
  while (v1 < n)
  {
    v1+=v2;
    tmp=v1; 
    v1=v2;
    v2=tmp;

    for(i=2; (i<=v1/2) || (printf("%d, \n", v1)==0); i++)
      if (v1%i==0) break;
  }
}

int main()
{
  fibprimes(30000);
  return(1);
}
