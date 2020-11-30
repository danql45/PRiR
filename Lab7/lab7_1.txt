#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "mpi.h"

#define STACJA_KOLEJOWA 1
#define ODJAZD 2
#define PODROZ 3
#define KONIEC_PODROZY 4
#define POMOC 5
#define TANKUJ 10000
#define REZERWA 1000

int p; /*liczba procesow*/
int nP; /*numer procesu*/
int ilePociagow;
int ilePeronow = 6;
int peronyZajete = 0;
int WJEDZ = 1, CZEKAJ = 0;
int ON = 10000; /*olej napedowy*/
int tag = 1;
int send[2];
int receive[2];
MPI_Status mpi_status;

void MPISend(int nrPociagu, int stan) {
    send[0] = nrPociagu;
    send[1] = stan;
    MPI_Send(&send, 2, MPI_INT, 0, tag, MPI_COMM_WORLD);
    sleep(1);
}

void StacjaKolejowa(int p) {
    int nrPociagu, stan;
    ilePociagow = p - 1;
    sleep(2);
    while(ilePeronow <= ilePociagow) 
    {
        MPI_Recv(&receive, 2, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &mpi_status);
        nrPociagu = receive[0];
        stan = receive[1];
        if(stan == 1) 
        {
            printf("Pociag nr %d stoi na stacji kolejowej\n", nrPociagu);
        }
        else if(stan == 2) 
        {
            printf("Pociag nr %d wyrusza w podroz z peronu %d\n", nrPociagu, peronyZajete);
            peronyZajete--;
        }
        else if(stan == 3) 
        {
            printf("Pociag nr %d jedzie\n", nrPociagu);
        }
        else if(stan == 4 || stan == 5) 
        {
            if(peronyZajete < ilePeronow) 
            {
                peronyZajete++;
                printf("Pociag przybyl na stacje\n");
                MPI_Send(&WJEDZ, 1, MPI_INT, nrPociagu, tag, MPI_COMM_WORLD);
            }
            else 
            {
                MPI_Send(&CZEKAJ, 1, MPI_INT, nrPociagu, tag, MPI_COMM_WORLD);
            }      
        }
    }
}

void Pociag() {
    int stan, i;
    stan = ODJAZD;
    while(1) {
        if(stan == 1) 
        {
            if(rand()%2 == 1) 
            {
                stan = ODJAZD;
                ON = TANKUJ;
                printf("Pociag nr %d prosi o pozwolenie na odjazd\n", nP);
                MPISend(nP, stan);
            }
            else 
            {
                MPISend(nP, stan);
            }
        }
        else if(stan == 2) 
        {
            printf("Wyruszylem w podroz, pociag nr %d\n", nP);
            stan = PODROZ;
            MPISend(nP, stan);
        }
        else if(stan == 3) 
        {
            ON -= rand()%500;
            if(ON <= REZERWA) {
                stan = KONIEC_PODROZY;
                printf("Pociag chce wjechac na stacje\n");
                MPISend(nP, stan);
            }
            else {
                for(i = 0; rand()%10000; i++);
            }            
        }
        else if(stan == 4)
        {
            int temp;
            MPI_Recv(&temp, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &mpi_status);
            if(temp == WJEDZ) {
                printf("Pociag nr %d wjezdza na stacje\n", nP);
                stan = STACJA_KOLEJOWA;
            }
            else 
            {
                ON -= rand()%1000;
                if(ON > 0) {
                    MPISend(nP, stan);
                }
                else {
                    stan = POMOC;
                    printf("Pociago nr %d koniec paliwa! Oczekuje na pomoc.\n", nP);
                    MPISend(nP, stan);
                }
            }
        }
        else if(stan == 5)
        {
            int temp;
            MPI_Recv(&temp, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &mpi_status);
            if(temp == WJEDZ) {
                stan = STACJA_KOLEJOWA;
                printf("Pociag nr %d otrzymał pomoc i wjezdza na stacje\n", nP);
            }
            else {
                printf("Czekam na pomoc. Pociag nr %d\n", nP);
            }
            MPISend(nP, stan);
        }
    }       
}

int main(int argc, char *argv[]) 
{
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &nP);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    srand(time(NULL));
    if(nP == 0)
        StacjaKolejowa(p);
    else
        Pociag();
    MPI_Finalize();
    return 0;
}
