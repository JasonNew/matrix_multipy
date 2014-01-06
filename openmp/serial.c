#include "stdio.h"
#include "stdlib.h"
#include "omp.h"
#define N 4000

void main()
{
    FILE *fp;
    int matrixa[N][N];
    int matrixb[N][N];
	int result[N][N] = {0};
    int row=N,col=N;
    
    fp = fopen("matrix4000", "rb");

    int i, j, k;
    for(i=0; i<N; i++)
    {
		for(j=0; j<N; j++)
        {
			fscanf(fp, "%d", &matrixa[i][j]);
			fscanf(fp, "%d", &matrixb[i][j]);
		}	
    }

	fclose(fp);
	
	double start = omp_get_wtime();
	for(i=0; i<N; i++)
	{
		for(j=0; j<N; j++)
		{
			for(k=0; k<N; k++)
			{
				result[i][j] = result[i][j] + matrixa[i][k] * matrixb[k][j];
			}
		}
	}
	double end = omp_get_wtime();
	printf("Cost time: %.6g\n", end - start);
}
