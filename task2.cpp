#include<mpi.h>
#include<iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int array[20];
    int rank, total_pr, N = 20;
    int result;
    MPI_Comm_size(MPI_COMM_WORLD, &total_pr);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int elements_per_process = N / total_pr;
    int *buffer_arr = new int[elements_per_process];
    int max_for_process;
    if(rank == 0) {
        for (int i = 0; i < 20; i++) {
            array[i] = i;
            printf("%d ", array[i]);
        }
    }
    array[3] = 50;
    MPI_Scatter(array, elements_per_process, MPI_INT, buffer_arr, elements_per_process, MPI_INT, 0, MPI_COMM_WORLD);
    max_for_process = 0;
    for (int i = 0; i < elements_per_process; i++)
        if (buffer_arr[i] > max_for_process) max_for_process = buffer_arr[i];

    MPI_Reduce(&max_for_process, &result, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
        printf("\nmaximum element of array is: %d \n", result);

    MPI_Finalize();
    return 0;

}