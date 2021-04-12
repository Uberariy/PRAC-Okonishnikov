#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *fin,*fout;
    char utf8sym1, utf8sym2, utf8sym3, bomle=1;
    int bad2=2;
    unsigned short utf16sym;

    if (argc >= 2)
    {
        if ((fin = fopen(argv[1], "r")) == NULL) 
            {fprintf(stderr, "ERROR 0: Unable to open input file\n"); return(1);}  
    } 
    else fin = stdin;

    if (argc >= 3)
    {
        if ((fout = fopen(argv[2], "w")) == NULL) 
            {fprintf(stderr, "ERROR 0: Unable to open output file\n"); return(2);} 
    } 
    else fout = stdout;

    bad2 = fread(&utf16sym, 2, 1, fin);
    if (utf16sym == 0xFFFE) bomle=0;
    if ((utf16sym != 0xFEFF) && (utf16sym != 0xFFFE))
    {
        fprintf(stderr, "ERROR 1: No BOM is found!\n");
    }
    else bad2 = fread(&utf16sym, 2, 1, fin);
    if (bad2 == 1) fprintf(stderr, "ERROR 2: Odd number of bytes!\n");

    while (bad2)
    {
        if (bomle == 0)
        {
            utf8sym1 = (char)(utf16sym / 256);
            utf16sym = utf16sym << 8;
            utf16sym += utf8sym1;
        }
        if (utf16sym < 128)
        {
            utf8sym1 = (char)utf16sym;
            fputc(utf8sym1, fout);
        }
        else if (utf16sym < 2048)
        {
            utf8sym2 = (char)(utf16sym % 64);
            utf8sym2 = utf8sym2 + 128;
            utf16sym = utf16sym >> 6;

            utf8sym1 = (char)(utf16sym);
            utf8sym1 = utf8sym1 + 128 + 64;           

            fputc(utf8sym1, fout);
            fputc(utf8sym2, fout);       
        }
        else
        {
            utf8sym3 = (char)(utf16sym % 64);
            utf8sym3 = utf8sym3 + 128;
            utf16sym = utf16sym >> 6;

            utf8sym2 = (char)(utf16sym % 64);
            utf8sym2 = utf8sym2 + 128;
            utf16sym = utf16sym >> 6;

            utf8sym1 = (char)(utf16sym);
            utf8sym1 = utf8sym1 + 128 + 64 + 32;

            fputc(utf8sym1, fout);
            fputc(utf8sym2, fout);
            fputc(utf8sym3, fout);    
        }
        bad2 = fread(&utf16sym, 2, 1, fin);
    }
    if (bad2 == 1) fprintf(stderr, "ERROR 2: Odd number of bytes!\n");
    
    printf("\n");
    fclose(fin);
    fclose(fout);

    return(0);
}