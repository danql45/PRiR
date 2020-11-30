#include <stdio.h>
#include <math.h>
#include "mpi.h"

float fp(float x) {
    return pow(x, 2);
}

int main(int argc, char **argv) {
    int p, n;
    int tag = 20, t = 0;
    float h, wynik = 0, calka = 0;
    float a = 1.4;
    float b = 2.9;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &p);
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    MPI_Status status;

    h = (b - a)/n;

    if(p == n-1) {
        printf("Procesow %d\n", p);
        printf("Przedział <%f, %f>\n\n", a, b);
        printf("Całkowanie x^2:\n");
        wynik = (fp(a) + fp(b))/2.0;
        MPI_Send(&h, 1, MPI_FLOAT, p - 1, tag, MPI_COMM_WORLD);
        MPI_Send(&wynik, 1, MPI_FLOAT, p - 1, tag, MPI_COMM_WORLD);
        MPI_Send(&t, 1, MPI_INT, p - 1, tag, MPI_COMM_WORLD);
    }
    if((p > 0) && (p < n-1)) {
        MPI_Recv(&h, 1, MPI_FLOAT, p + 1, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&wynik, 1, MPI_FLOAT, p + 1, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&t, 1, MPI_INT, p + 1, tag, MPI_COMM_WORLD, &status);
        wynik += fp(a + t*h);
        calka = wynik * h;
        t++;
        if(p != 0) {            
            MPI_Send(&h, 1, MPI_FLOAT, p - 1, tag, MPI_COMM_WORLD);
            MPI_Send(&wynik, 1, MPI_FLOAT, p - 1, tag, MPI_COMM_WORLD);
            MPI_Send(&t, 1, MPI_INT, p - 1, tag, MPI_COMM_WORLD);
        }  
        printf("Wynik: %f, proces Nr: %d\n",calka , p);
    }
    if(p == 0) {
        MPI_Recv(&h, 1, MPI_FLOAT, p + 1, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&wynik, 1, MPI_FLOAT, p + 1, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&t, 1, MPI_INT, p + 1, tag, MPI_COMM_WORLD, &status);
        wynik += fp(a + t*h);
        calka = wynik * h;
        printf("Wynik: %f, proces Nr: %d\n",calka , p);
    }
    MPI_Finalize();
    return 0;
}
