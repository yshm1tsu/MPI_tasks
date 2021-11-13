#include<mpi.h>
#include<iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int array[1500];
    int rank, total_pr, N = 1500;
    double start_time_send, end_time_send;

    MPI_Comm_size(MPI_COMM_WORLD, &total_pr);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            array[i] = rand() % 2500;
        }

        start_time_send = MPI_Wtime();
        MPI_Send(array, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
        end_time_send = MPI_Wtime();
        printf("Send time %f\n", end_time_send - start_time_send);
    } else {
        MPI_Recv(array, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    if (rank == 0) {
        start_time_send = MPI_Wtime();
        MPI_Ssend(array, N, MPI_INT, 1, 1, MPI_COMM_WORLD);
        end_time_send = MPI_Wtime();
        printf("Ssend time %f\n", end_time_send - start_time_send);
    } else {
        MPI_Recv(array, N, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    if (rank == 0) {
        start_time_send = MPI_Wtime();
        MPI_Rsend(array, N, MPI_INT, 1, 2, MPI_COMM_WORLD);
        end_time_send = MPI_Wtime();
        printf("rsend time %f\n", end_time_send - start_time_send);
    } else {
        MPI_Recv(array, N, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    MPI_Finalize();
    return 0;
}