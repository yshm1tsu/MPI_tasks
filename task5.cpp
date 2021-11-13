#include<mpi.h>
#include<iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    const int N = 10;
    int vector_1[N], vector_2[N], process_multiplication_sum = 0, multiplication_sum = 0;
    int rank, total_pr;
    MPI_Comm_size(MPI_COMM_WORLD, &total_pr);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank == 0) {
        for (int i = 0; i < N; i++) {
            vector_1[i] = rand() % 30;
            vector_2[i] = rand() % 40;

            printf("%d ", vector_1[i]);
            printf("%d ", vector_2[i]);
        }
    }

    int *count_send = new int[total_pr];
    int *indent = new int[total_pr];

    int rest = N;
    int k = rest / total_pr;
    count_send[0] = k;
    indent[0] = 0;

    for (int i = 1; i < total_pr; i++) {
        rest -= k;
        k = rest / (total_pr - i);
        count_send[i] = k;
        indent[i] = indent[i - 1] + count_send[i - 1];
    }

    int proc_num = count_send[rank];
    int *vector_1_process = new int[proc_num];
    int *vector_2_process = new int[proc_num];

    MPI_Scatterv(vector_1, count_send, indent, MPI_INT, vector_1_process, proc_num,
                 MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(vector_2, count_send, indent, MPI_INT, vector_2_process, proc_num,
                 MPI_INT, 0, MPI_COMM_WORLD);

    process_multiplication_sum = 0;
    for (int i = 0; i < proc_num; i++) {
        process_multiplication_sum += vector_1_process[i] * vector_2_process [i];
    }

    MPI_Reduce(&process_multiplication_sum, &multiplication_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("\nscalar multiplication: %d", multiplication_sum);
    }

    MPI_Finalize();
    return 0;

}