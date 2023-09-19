#include <iostream>
#include <cstdlib>
#include <chrono>
#include <mpi.h>

using namespace std::chrono;

void randomVector(int vector[], int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 100;
    }
}

int main(int argc, char **argv) {
    unsigned long size = 100000000;

    int numtasks, rank;

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the number of tasks/processes
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    // Get the rank
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int local_size = size / numtasks;
    int *v1, *v2, *v3, *local_v1, *local_v2, *local_v3;

    // Allocate memory space
    v1 = new int[size];
    v2 = new int[size];
    v3 = new int[size];
    local_v1 = new int[local_size];
    local_v2 = new int[local_size];
    local_v3 = new int[local_size];

    // Root process creates an array of random numbers
    if (rank == 0) {
        randomVector(v1, size);
        randomVector(v2, size);
    }

    // Synchronize before starting the timer
    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    // Use scatter to distribute work among all worker processes
    MPI_Scatter(v1, local_size, MPI_INT, local_v1, local_size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(v2, local_size, MPI_INT, local_v2, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Addition done locally on each process
    for (int i = 0; i < local_size; i++) {
        local_v3[i] = local_v1[i] + local_v2[i];
    }

    // Use gather to collect local_v3 results back to the root process (v3)
    MPI_Gather(local_v3, local_size, MPI_INT, v3, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Record end time
    MPI_Barrier(MPI_COMM_WORLD);
    double end_time = MPI_Wtime();

    if (rank == 0) {
        // Calculate execution time in milliseconds
        double execution_time = 1000.0 * (end_time - start_time);
        std::cout << "\nTotal time taken by function: " << execution_time << " milliseconds" << std::endl;
    }

    // Free allocated memory
    // if : malloc you then have to call free to free memory
    // if : new you have to call delete to free memory
    delete[] v1;
    delete[] v2;
    delete[] v3;
    delete[] local_v1;
    delete[] local_v2;
    delete[] local_v3;

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}
