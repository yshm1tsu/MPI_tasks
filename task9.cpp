#include<mpi.h>
#include<iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, total_pr, N = 17;
    int* array = new int[N];
    int* result_array = new int[N];

    MPI_Comm_size(MPI_COMM_WORLD, &total_pr);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            array[i] = rand() % 100;
            printf("%d ", array[i]);
        }
        printf("\n");
    }

    int* count_send = new int[total_pr];
    int* indent = new int[total_pr];
    int* reversed_int = new int[total_pr];

    int rest = N;
    int k = rest / total_pr;
    count_send[0] = k;
    indent[0] = 0;
    reversed_int[0] = N - k;

    for (int i = 1; i < total_pr; i++) {
        rest -= k;
        k = rest / (total_pr - i);
        count_send[i] = k;
        indent[i] = indent[i - 1] + count_send[i - 1];
        reversed_int[i] = reversed_int[i - 1] - count_send[i];
    }
    int process_length = count_send[rank];
    int* buffer_array = new int[process_length];

    MPI_Scatterv(array, count_send, indent, MPI_INT, buffer_array, process_length, MPI_INT, 0, MPI_COMM_WORLD);

    int* reverse = new int[process_length];
    for(int i = 0; i < process_length; i++) {
        reverse[i] = buffer_array[process_length - i - 1];
    }

    MPI_Gatherv(reverse, process_length, MPI_INT, result_array, count_send, reversed_int, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0) {
        printf("\n");
        for(int i = 0; i < N; i++)
            printf("%d ", result_array[i]);
    }

    MPI_Finalize();
    return 0;
}