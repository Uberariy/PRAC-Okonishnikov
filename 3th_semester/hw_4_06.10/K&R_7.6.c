#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char *a=NULL,*b=NULL;
    int i,u=1,v=1;
    FILE *fa,*fb;
    if ((fa = fopen(*++argv, "r")) == NULL) 
        {printf("%s\n", "Не могу открыть первый файл"); return(1);}   
    if ((fb = fopen(*++argv, "r")) == NULL)        
        {printf("%s\n", "Не могу открыть второй файл"); return(1);}  
    while (u || v)
    {   
        a = malloc(1);
        b = malloc(1);
        i=0;
        while (((a[i]=getc(fa)) != '\n') && (u=(a[i] != EOF)))
            a = realloc(a, 2 + i++);
        a[i] = '\0';

        i=0;
        while (((b[i]=getc(fb)) != '\n') && (v=(b[i] != EOF)))
            b = realloc(b, 2 + i++);
        b[i] = '\0';

        if (strcmp(a,b) !=  0)
        {
            printf("%s\n%s\n%s\n", "Впервые различающиеся строки:",a,b);
            free(a);    free(b);
            return(0);
        }
        free(a);    free(b);
    }
    printf("%s\n", "Все строки одинаковы!");
    return(0);
}
