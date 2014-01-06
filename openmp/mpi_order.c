#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include "omp.h"

#define NRA 4000                /* number of rows in matrix A */
#define NCA 4000                 /* number of columns in matrix A */
#define NCB 4000                  /* number of columns in matrix B */
#define MASTER 0               /* taskid of first task */
#define FROM_MASTER 1          /* setting a message type */
#define FROM_WORKER 2          /* setting a message type */

int a[NRA][NCA],           /* matrix A to be multiplied */
	b[NCA][NCB],           /* matrix B to be multiplied */
	c[NRA][NCB]={0};           /* result matrix C */

int main (int argc, char *argv[])
{
int	numtasks,              /* number of tasks in partition */
	taskid,                /* a task identifier */
	numworkers,            /* number of worker tasks */
	source,                /* task id of message source */
	dest,                  /* task id of message destination */
	mtype,                 /* message type */
	rows,                  /* rows of matrix A sent to each worker */
	averow, extra, offset, /* used to determine rows sent to each worker */
	i, j, k, rc;           /* misc */

MPI_Status status;

MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
if (numtasks < 2 ) {
  printf("Need at least two MPI tasks. Quitting...\n");
  MPI_Abort(MPI_COMM_WORLD, rc);
  exit(1);
}
numworkers = numtasks-1;


FILE *fp;
fp = fopen("matrix4000", "rb");
/**************************** master task ************************************/
if (taskid == MASTER)
{
  printf("mpi_mm has started with %d tasks.\n",numtasks);
  printf("Initializing arrays...\n");

  for(i=0; i<NRA; i++)
  {
    for(j=0; j<NRA; j++)
    {
  		fscanf(fp, "%d", &a[i][j]);
  		fscanf(fp, "%d", &b[i][j]);
    }	
  }

	
	double start = omp_get_wtime();
  /* Send matrix data to the worker tasks */
  averow = NRA/numworkers;
  extra = NRA%numworkers;
  offset = 0;
  mtype = FROM_MASTER;
  for (dest=1; dest<=numworkers; dest++)
  {
     rows = (dest <= extra) ? averow+1 : averow;   	
     MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
     MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
     MPI_Send(&a[offset][0], rows*NCA, MPI_INT, dest, mtype,
               MPI_COMM_WORLD);
     MPI_Send(&b, NCA*NCB, MPI_INT, dest, mtype, MPI_COMM_WORLD);
     offset = offset + rows;
  }

  /* Receive results from worker tasks */
  mtype = FROM_WORKER;
  for (i=1; i<=numworkers; i++)
  {
     source = i;
     MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
     MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
     MPI_Recv(&c[offset][0], rows*NCB, MPI_INT, source, mtype, 
              MPI_COMM_WORLD, &status);
  }

	double end = omp_get_wtime();
	printf("Cost Time %.6g\n", end-start);
  printf ("Done.\n");
  }


/**************************** worker task ************************************/
   if (taskid > MASTER)
   {
      mtype = FROM_MASTER;
      MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&a, rows*NCA, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&b, NCA*NCB, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);

      #pragma omp parallel for private(i,j,k) num_threads(32)
      for (i=0; i<rows; i++)
      {
         for (k=0; k<NCA; k++)
         {
            for (j=0; j<NCA; j++)
            {
					   c[i][j] = c[i][j] + a[i][k] * b[k][j];
			       }
         }
	    }
      mtype = FROM_WORKER;
      MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
      MPI_Send(&c, rows*NCB, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
   }
  MPI_Finalize();
}
