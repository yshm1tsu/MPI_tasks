#include<mpi.h>
#include<iostream>


int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int N = 20;
    int rank, size, proc_n, proc_min_n;
    int a[N * N];
    int *process_a, *process_minimum_a;
    int maximum_minimum;
    int *local_minimum = new int[N];
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            int localmin = 1000;
            for (int j = 0; j < N; j++) {

                int f = rand() % 1000;
                a[i * N + j] = f;

                if (f < localmin)
                    localmin = f;

                printf("\t%d\t", a[i * N + j]);

            }
            printf(" local min: %d \n", localmin);
            local_minimum[i] = 1000;
        }

    }
    int *count_send = new int[size];
    int *indent = new int[size];

    int *count_sen_min = new int[size];
    int *indent_min = new int[size];

    int rest = N;
    int k = rest / size;
    count_send[0] = k * N;
    indent[0] = 0;

    count_sen_min[0] = k;
    indent_min[0] = 0;

    for (int i = 1; i < size; i++) {
        rest -= k;
        k = rest / (size - i);
        count_send[i] = k * N;
        indent[i] = indent[i - 1] + count_send[i - 1];

        count_sen_min[i] = k;
        indent_min[i] = indent_min[i - 1] + count_sen_min[i - 1];
    }

    proc_n = count_send[rank];
    process_a = new int[proc_n];

    proc_min_n = count_sen_min[rank];
    process_minimum_a = new int[proc_min_n];

    MPI_Scatterv(a, count_send, indent, MPI_INT, process_a, proc_n, MPI_DOUBLE,
                 0, MPI_COMM_WORLD);

    MPI_Scatterv(local_minimum, count_sen_min, indent_min, MPI_INT, process_minimum_a, proc_min_n, MPI_DOUBLE,
                 0, MPI_COMM_WORLD);
    int local_max = 0;
    for (int i = 0; i < proc_n / N; i++) {
        int min = process_a[i * N];
        for (int j = 0; j < N; j++) {
            if (min > process_a[i * N + j]) {
                min = process_a[i * N + j];
            }
        }
        if (local_max < min) {
            local_max = min;
        }

    }
    int buf[1] = {local_max};

    MPI_Reduce(buf, &maximum_minimum, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("maximum_minimum is: %d \n", maximum_minimum);
    }


    MPI_Finalize();
    return 0;
}
