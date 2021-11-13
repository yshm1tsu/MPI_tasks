#include<mpi.h>
#include<iostream>

using namespace std;

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, total_pr;

    MPI_Comm_size(MPI_COMM_WORLD, &total_pr);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *send = new int[1];
    int *recieve = new int[1];
    if (rank == 0) {
        send[0] = 100;
        MPI_Send(send, 1, MPI_INT, rank + 1, rank + 1, MPI_COMM_WORLD);
        printf("value = %d, rank = %d \n", send[0], rank);
    }
    if (rank != 0) {
        MPI_Recv(recieve, 1, MPI_INT, rank - 1, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        send[0] = 20 + recieve[0];
        printf("value = %d, rank = %d \n", send[0], rank);
        if (rank == total_pr - 1) {
            MPI_Send(send, 1, MPI_INT, 0, rank + 1, MPI_COMM_WORLD);
        } else {
            MPI_Send(send, 1, MPI_INT, rank + 1, rank + 1, MPI_COMM_WORLD);
        }

    }

    MPI_Finalize();
    return 0;
}