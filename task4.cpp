#include<mpi.h>
#include<iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int N = 30;
    int rank, total_pr;
    int array[N];
    int sum_array[2], sum_process_count[2];
    MPI_Comm_size(MPI_COMM_WORLD, &total_pr);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank == 0) {
        for (int i = 0; i < N; i++) {
            array[i] = -20 + rand() % 50;
            printf("%d ", array[i]);
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
    int *proc_part = new int[proc_num];

    MPI_Scatterv(array, count_send, indent, MPI_INT, proc_part, proc_num,
                 MPI_INT, 0, MPI_COMM_WORLD);

    sum_process_count[0] = 0;
    sum_process_count[1] = 0;

    for (int i = 0; i < proc_num; i++) {
        if (proc_part[i] > 0) {
            sum_process_count[0] += proc_part[i];
            sum_process_count[1]++;
        }
    }

    MPI_Reduce(&sum_process_count, &sum_array, 2, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("%d %d", sum_array[0], sum_array[1]);
        double result = (double) sum_array[0] / sum_array[1];
        printf("\nresult is: %f", result);
    }

    MPI_Finalize();
    return 0;

}