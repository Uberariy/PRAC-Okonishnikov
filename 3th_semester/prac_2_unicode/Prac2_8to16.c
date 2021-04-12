#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *fin,*fout;
    unsigned char utf8sym, suit=0;
    int offset=0, next;
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
            {fprintf(stderr, "ERROR 1: Unable to open output file\n"); return(2);} 
    } 
    else fout = stdout;

    while (fread(&utf8sym, sizeof(char), 1, fin))
    {
        utf16sym=0;
        offset++;
        if ((utf8sym >> 7) == 0)
        {
            utf16sym += utf8sym;
            suit = 1;
        }
        else if ((utf8sym >> 5) == 6)
        {
            utf8sym = utf8sym & 31;
            utf16sym += utf8sym;
            utf16sym = utf16sym << 6;
            fread(&utf8sym, sizeof(char), 1, fin);  
            offset++;

            if ((utf8sym >> 6) == 2)
            {
                utf8sym = utf8sym & 63;
                utf16sym += utf8sym;
                suit = 1;
            }
            else fprintf(stderr, "\nERROR 2: Wrong character in %d byte!\n", offset);
        }
        else if ((utf8sym >> 4) == 14)
        {
            utf8sym = utf8sym & 15;
            utf16sym += utf8sym;
            utf16sym = utf16sym << 6;
            fread(&utf8sym, sizeof(char), 1, fin);
            offset++;

            if ((utf8sym >> 6) == 2)
            {
                utf8sym = utf8sym & 63;
                utf16sym += utf8sym;
                utf16sym = utf16sym << 6;
                fread(&utf8sym, sizeof(char), 1, fin);
                offset++;

                if ((utf8sym >> 6) == 2)
                {
                    utf8sym = utf8sym & 63;
                    utf16sym += utf8sym;
                    suit = 1;
                }
                else fprintf(stderr, "\nERROR 3: Wrong character in %d byte!\n", offset);
            }
            else fprintf(stderr, "\nERROR 4: Wrong character in %d byte!\n", offset);
        }
        else fprintf(stderr, "\nERROR 5: Wrong character in %d byte!\n", offset);
        if (suit)
        {
            fwrite(&utf16sym, sizeof(utf16sym), 1, fout);
            suit = 0;
        }
    }
    
    printf("\n");
    fclose(fin);
    fclose(fout);

    return(0);
}