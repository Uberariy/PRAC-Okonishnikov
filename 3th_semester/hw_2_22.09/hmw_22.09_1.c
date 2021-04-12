#include <stdio.h>
#define N 10

void swap(int *a, int *b)
{
    int c=*a;    *a=*b;    *b=c;
}

void remap(int a[N]) // Функция, переставляющая числа в целом массиве.
{
    int i=0,j;

    for (j=1; j<N; j++)
    {   
        if (a[i]>=0)
        {
            while ((a[j]>=0) && (j<N)) j++;
            if (a[j]<0) swap(&a[i],&a[j]);
        } 
        i++;
    }

    for (j=i+1; j<N; j++)
    {   
        if (a[i]>0)
        {
            while ((a[j]>0) && (j<N)) j++;
            if (a[j]==0) swap(&a[i],&a[j]);
        }
        i++; 
    }
}

int main()
{
    int k, a[N];

    printf("\n%s\n","Enter");
    for (k=0; k<N; k++)
        scanf("%d",&a[k]);

    remap(&a[0]);

    for (k=0; k<N; k++)
        printf("%d\t", a[k]);   

    return(1);
}
