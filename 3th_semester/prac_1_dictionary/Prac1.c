#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct tree
{
    char* word;
    int number;
    struct tree *left;
    struct tree *right;
};

struct tree *addnode (struct tree *T, char *w, int size, int *maxN)
{
    int comp;
    if (T==NULL)    
    {
        T = (struct tree *)malloc(sizeof(struct tree));
        T -> word = (char *)malloc(size+1);
        strcpy (T -> word, w);
        T -> number = 1;
        T -> left = T -> right = NULL;
        (*maxN)=1;
    }
    else if ((comp = strcmp(w, T -> word)) == 0)
    {
        T -> number++;
    }
    else if (comp < 0)
        T -> left = addnode(T -> left, w, size, maxN);
    else
        T -> right = addnode(T -> right, w, size, maxN);
    if ((*maxN) < (T -> number)) (*maxN) = T -> number;
    return T;
}

void printree (struct tree *T, int i, double freq, FILE *fout)
{
    if (T != NULL)
    {
        printree (T -> left, i, freq, fout);
        if (i == T -> number)
            fprintf(fout, "%s  %d  %6f\n", T -> word, i, freq);
        printree (T -> right, i, freq, fout);    
    }
}

void printbynumber (struct tree *T, int *maxN, int wholeN, FILE *fout)
{
    int i;
    for (i=(*maxN); i>=1; i--)
        printree(T,i,(double)i/wholeN, fout);
}

void freetree (struct tree *T)
{
    if (T != NULL)
    {
        freetree (T -> left);
        freetree (T -> right);
        free (T -> word); 
        free (T);
    }
}

int main(int argc, char *argv[])
{
    struct tree *T=NULL;
    char* w = NULL;
    char c;
    int size = 0, wholeN = 0;
    int maxN = 0;
    FILE *fin,*fout;

    ++argv;

    if ((*argv != 0) && (strcmp("-i",*argv) == 0))
    {
        if ((fin = fopen(*++argv, "r")) == NULL) 
            {printf("%s\n", "Unable to open input file"); return(1);}  
        ++argv;
    } 
    else fin = stdin;

    if ((*argv != 0) && (strcmp("-o",*argv) == 0))
    {
        if ((fout = fopen(*++argv, "w")) == NULL) 
            {printf("%s\n", "Unable to open output file"); return(1);} 
        ++argv; 
    } 
    else fout = stdout;

    w = malloc(2);
    while ((c = getc(fin)) != EOF)
    {      
        while (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')))
        {
            w[size] = c;
            size++;
            w = realloc(w, size+1);               
            c = getc(fin);
        }
        if (size>0) 
        {
            w[size] = '\0';
            T = addnode(T,w,size,&maxN); 
            size=0;    
            free(w);     
            w = malloc(2);
            wholeN++;
        }
        if ((c == '.') || (c == ',') || (c == ';') || (c == '!') || (c == '?') || (c == ':'))
        {
            w[size] = c;
            size++;
            
            w[size] = '\0';    
            T = addnode(T,w,1,&maxN); 
            size=0;    
            free(w);     
            w = malloc(2);
            wholeN++;           
        }   
    }
    if (w != NULL) free(w);

    printf("\t%s\n","Printing starts...");
    printbynumber(T, &maxN, wholeN, fout);
    printf("\t%s\n","Printing ends.");
    freetree(T);
    fclose(fin);
    fclose(fout);

    return(0);
}