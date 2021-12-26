#include <stdio.h>
#include <time.h> 
#include <stdlib.h>
#include <omp.h>

double det(int N, int threadsNum)
{
	omp_set_num_threads(threadsNum);
	int i,j,k;
	double **A, tmp;
    A = (double**)malloc(sizeof(double*)*N);
    for(int i = 0; i < N; i++) {
        A[i] = (double*)malloc(sizeof(double)*N);
    }
	srand(42);
	#pragma omp parallel
	#pragma for
	for (i = 0; i < N; i++ ) 
	{
		for (j = 0; j < N; j++ ) 
		{
			A[i][j] = (double)(rand()%10000)/((double)12000);
		}
	}
	for (i = 0; i < N; i++) 
	{
		for (j = 0; j < N; j++) 
		{
//			printf("%f ", A[i][j]);
		}
//		puts("\n");
	}

	double *L;
	L = (double*)malloc(sizeof(double)*N);
	for (i = 0; i < N; i++) 
	{ 	
		for (k = i; k < N; k++)
		{
			if (A[k][i] != 0)
			{
				for (j = 0; j < N; j++)
				{
					L[j] = A[k][j];
				}
				break;
			}
		}
		double *Weights;
		Weights = (double*)malloc(sizeof(double)*N);
		#pragma omp parallel for
		for (k = i+1; k < N; k++)
		{
			Weights[k] = A[k][i];
		}
    	//omp_set_num_threads(threadsNum);
		#pragma omp parallel for private(tmp, j) //shared(A, L, N) private(k, j, tmp)
		for (k = i+1; k < N; k++)
		{
			tmp = L[i] / Weights[k];
			if (A[k][i] != 0)
			{
				for (j = 0; j < N; j++) 
				{
	//				fprintf(stderr, "*%f, k: %d, j: %d, i: %d\n", tmp, k, j, i);
					A[k][j] -= L[j] / tmp;
				}	
			}		
		}
	
	}

	for (i = 0; i < N; i++) 
	{
		for (j = 0; j < N; j++) 
		{
		//	printf("%f ", A[i][j]);
		}
		//puts("\n");
	}
	double det=1;
	for (i = 0; i < N; i++) 
		det *= A[i][i];
	return det;
}

int main()
{
	int i, j, k;
	int N[5] = {100, 500, 1000, 2000, 5000};
	int threadsNum[6] = {40, 60, 80, 100, 120, 140};
	double start1, finish1, runing_t, res;

	for (i = 0; i < 5; i++) 
	{
		for (j = 0; j < 6; j++) 
		{
			runing_t = 0;
			for (k = 0; k < 3; k++)
			{
				start1 = omp_get_wtime();

				res = det(N[i], threadsNum[j]);

				finish1 = omp_get_wtime();
				runing_t += finish1-start1;
			}
			printf ("Size: %d, \t, Threads: %d, \tTime: %f s, \tResult: %f\n", N[i], threadsNum[j], runing_t/((double)3), res);
		}
	}
}