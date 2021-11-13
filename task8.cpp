#include<mpi.h>
#include<iostream>
#include <algorithm>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int N = 20;
    int array[N];
    int rank, total_pr = N;
    MPI_Comm_size(MPI_COMM_WORLD, &total_pr);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int elements_per_process = N / total_pr;
    int *subarray = new int[elements_per_process];
    int new_array[elements_per_process];
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            array[i] = rand() % 25;
            printf("%d ", array[i]);
        }
        printf("\n");
        for (int i = 0; i < N; i += elements_per_process) {
            MPI_Send(array + i, elements_per_process, MPI_INT, i / elements_per_process, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Recv(subarray, elements_per_process, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    for (int i = 0; i < elements_per_process; i++) {
        printf(" %d", subarray[i]);
    }
    printf(" : from process %d\n", rank);
    MPI_Send(subarray, elements_per_process, MPI_INT, 0, 0, MPI_COMM_WORLD);


    if (rank == 0) {
        for (int i = 0; i < total_pr; ++i) {
            MPI_Recv(new_array, elements_per_process, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int j = 0; j < elements_per_process; ++j) {
                printf(" %d ", new_array[j]);
            }
        }
    }

    MPI_Finalize();
    return 0;
}