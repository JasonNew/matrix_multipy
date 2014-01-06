#include "stdio.h"
#include "stdlib.h"
#include "omp.h"

#define N 4000
int main()
{
    FILE *fp;
    int matrix[N][N] = {0};
	int result[N][N] = {0};
    int row=N,col=N;
    
    fp = fopen("matrix4000", "rb");

    int i, j, k;
    for(i=0; i<N; i++)
    {
		for(j=0; j<N; j++)
        {
			fscanf(fp, "%d", &matrix[i][j]);
		}	
    }

	fclose(fp);
	
	double start = omp_get_wtime();
	#pragma omp parallel for private(i,j,k) num_threads(32)
	for(i=0; i<N; i++)
	{
		for(k=0; k<N; k++)
		{
			for(j=0; j<N; j++)
			{
				result[i][j] = result[i][j] + matrix[i][k] * matrix[k][j];
			}
		}
	}
	double end = omp_get_wtime();
	printf("Cost time: %.6g\n", end - start);
	return 0;
}
