#include <stdio.h>
#include <time.h> 
#include <stdlib.h>
#include <mpi.h>

double det(int N, int myrank, int ranksize)
{
	//fprintf(stderr, "|%d %d|", myrank, ranksize);
	int i,j,k;
	double **A, tmp;
    A = (double**)malloc(sizeof(double*)*N);
    for(int i = 0; i < N; i++) {
        A[i] = (double*)malloc(sizeof(double)*N);
    }
	srand(42);

	for (i = 0; i < N; i++ ) 
	{
		for (j = 0; j < N; j++ ) 
		{
			A[i][j] = (double)(rand()%10000)/((double)12000);
		}
	}
	
	if (0)
	for (i = 0; i < N; i++) 
	{
		for (j = 0; j < N; j++) 
		{
			printf("%f ", A[i][j]);
		}
		puts("\n");
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

		if (myrank==0)
		for (k = i+1; k < N; k++)
		{
			Weights[k] = A[k][i];
		}
		MPI_Bcast(&Weights[i+1], N-i-1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

		for (k = myrank; k < N; k += ranksize)
		{
			if (k < i+1)
				continue;
			tmp = L[i] / Weights[k];
			if (A[k][i] != 0)
			{
				for (j = 0; j < N; j++) 
				{
					//fprintf(stderr, "*%f * %f k: %d, j: %d, i: %d myrank: %d\n", L[j] / tmp, A[k][j], k, j, i, myrank);
					A[k][j] -= L[j] / tmp;
				}	
			}		
		}
		for (k = i+1; k < N; k++)
		{
			MPI_Bcast(&A[k][0], N, MPI_DOUBLE, k % ranksize, MPI_COMM_WORLD);
		}
	}

	if (0)
	for (i = 0; i < N; i++) 
	{
		for (j = 0; j < N; j++) 
		{
			printf("%f ", A[i][j]);
		}
		puts("\n");
	}
	double det=1;
	for (i = 0; i < N; i++) 
		det *= A[i][i];
	
	free(L);
	free(A);
	return det;
}

int main(int argc, char **argv)
{
	int ranksize, myrank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ranksize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	int i, j, k;
	int N[5] = {1000, 500, 1000, 2000, 5000};
	double start1, finish1, runing_t, res;

	for (i = 0; i < 1; i++) 
	{
		runing_t = 0;
		for (k = 0; k < 1; k++)
		{
			if (myrank == 0)
				start1 = MPI_Wtime();

			res = det(N[i], myrank, ranksize);
			fprintf(stderr, "res: %f\n", res);

			if (myrank == 0)
			{
				finish1 = MPI_Wtime();
				runing_t += finish1-start1;
			}
		}
		if (myrank == 0)
			printf ("Size: %d, \t, Processes: %d, \tTime: %f s, \tResult: %f\n", N[i], ranksize, runing_t/((double)1), res);
	}
	MPI_Finalize();
}