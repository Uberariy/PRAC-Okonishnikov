// rdtsc.cpp
// processor: x86, x64
#include <stdio.h>
#include <x86intrin.h>
#define SIZE 1024
double a[SIZE][SIZE];
int i;
double  b[SIZE][SIZE];
double  c[SIZE][SIZE];
double  b2[SIZE];
double  c2[SIZE];
void main( int argc, char **argv )
{
    unsigned long u;
    u = __rdtsc();

  int j,k;
  for( i=0; i < SIZE; i++ )
  { 
    for( j = 0; j < SIZE; j++ )
    {
       b[i][j] = 2.0;
       c[i][j] = 1.9; 
    }
  }
  for( j = 0; j < SIZE; j++ )
  {
      for( i = 0; i < SIZE; i++ )
      {
          b2[j] = b[j][i];
          c2[j] = c[i][j];
      }
  }
  for( j = 0; j < SIZE; j++ )
  {
    for( i = 0; i < SIZE; i++ )
    {
      a[i][j] = 0;      
      a[i][j] += b2[i]+c2[j];
    }
  }

    unsigned long l;
    l = __rdtsc();
    printf("%ld ticks\n", l-u);
}
