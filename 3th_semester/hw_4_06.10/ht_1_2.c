#include <stdio.h>
#include <stdlib.h>
#define EOL '\n'

int main()
{
	char* mas1=NULL;
	char* masmax=NULL;
	char c;
	int size1=1,numb1=0,maxnumb=0,i;
	 

	while (((c=getchar()) != EOF))
	{
		numb1++;
		mas1=malloc(1);
		mas1[0]=c;
				
		while (((c=getchar()) != EOL) && (c != EOF))  // Чтение одной строки, ее запись в mas1
		{
		if (numb1==size1)
			{
				size1=size1*2+1;
				mas1=realloc(mas1, size1);
			}
		mas1[numb1]=c;
		numb1++;
		}
		
//        printf("%d \t %d \t %d \n", numb1, maxnumb, size1); // Отладка размеров
		if (numb1>maxnumb)  // Запись mas1 в masmax, если mas1 оказался больше masmax
		{
			maxnumb=numb1;
			masmax=realloc(masmax, size1);
			for (i=0; i<maxnumb; i++)
				masmax[i]=mas1[i];	
		}
		
		free(mas1);		
		numb1=0;		
		size1=1;
	}
	
	printf("\n");
	for (i=0; i<maxnumb; i++)
		putchar(masmax[i]);
	printf("\n");
		
	free(masmax);
	return 0;
}

