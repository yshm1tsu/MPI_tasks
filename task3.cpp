#include<mpi.h>
#include<iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int count = 1000000;
    int rank, comm_size;
    int counter_in_pr = 0;
    double x, y;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    srand(rank);
    int balance = count / comm_size;
    int counter = 0;
    for (int i = rank * balance; i < rank * balance + balance; i++)
    {
        x = (double)rand() / (double)0x7fffffff * 2 - 1;
        y = (double)rand() / (double)0x7fffffff * 2 - 1;
        if (pow(x, 2) + pow(y, 2) <= 1)
            counter++;
    }
    MPI_Reduce(&counter, &counter_in_pr, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        double answer = (double)(4 * counter_in_pr) / (double)count;
        printf("Pi = %f", answer);
    }
    MPI_Finalize();

}