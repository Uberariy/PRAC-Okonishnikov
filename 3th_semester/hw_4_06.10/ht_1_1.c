#include <stdio.h>

int a[1024], i, j, n;

void change(int *a, int *b)
{
    int c;
    c=*a;    *a=*b;    *b=c;
}

int main()
{

    printf("\n%s","Enter number of array elements: ");
    scanf("%d\n",&n);
    for (i=0; i<n; i++)
        scanf("%d",&a[i]);
    
    for (i=n-1; i>0; i--)
        for (j=1; j<=i; j++)
            if (a[j]<a[j-1])  
                change(&a[j],&a[j-1]);

    for (i=0; i<n; i++)
        printf("%d\t",a[i]);

return 0;
}
