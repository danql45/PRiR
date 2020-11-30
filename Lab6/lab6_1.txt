#include <stdio.h>
#include <math.h>
#include "mpi.h"
int main(int argc, char **argv) 
{
    int p, n, tag = 50;
    float pi = 4, sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &p);
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    MPI_Status status;

    if(p == 0) 
    {
        MPI_Send(&sum, 1, MPI_DOUBLE, p + 1, tag, MPI_COMM_WORLD);
    }

    if((p > 0) && (p <= n-1)) 
    {
        MPI_Recv(&sum, 1, MPI_DOUBLE, p - 1, tag, MPI_COMM_WORLD, &status);
        sum += pow(-1,p - 1)/(2*p-1);
        pi *= sum;
        printf("Proces Nr: %d Przyblizenie PI = %f\n", p, pi);
        if(p != n-1)
        {
            MPI_Send(&sum, 1, MPI_DOUBLE, p + 1, tag, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
