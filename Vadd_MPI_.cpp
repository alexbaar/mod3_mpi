#include <iostream>

#include <cstdlib>

#include <time.h>

#include <chrono>

#include <mpi.h>



using namespace std::chrono;

using namespace std;



void randomVector(int vector[], int size)

{

    for (int i = 0; i < size; i++)

    {

        // populate with random numbers

        vector[i] = rand() % 100;

    }

}



int main(int argc, char **argv)

{

    unsigned long size = 100000000;



    int numtasks, rank, name_len, tag = 1;

    char name[MPI_MAX_PROCESSOR_NAME];



    int msgLen = 50;



    // Initialize the MPI environment

    MPI_Init(&argc, &argv);



    // Get the number of tasks/processes

    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);



    // Get the rank

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);



    // Find the processor name

    MPI_Get_processor_name(name, &name_len);



    srand(time(0));



    int *v1, *v2, *v3, *local_v1, *local_v2, *local_v3;

    int local_size = size / numtasks;



// record start time

    double start_time = MPI_Wtime();



    // Allocate memory space

    v1 = (int *)malloc(size * sizeof(int));

    v2 = (int *)malloc(size * sizeof(int));

    v3 = (int *)malloc(size * sizeof(int));

    // for performing addition locally - no racing condition as we dont try accessing a shared var

    local_v1 = (int *)malloc(local_size * sizeof(int));

    local_v2 = (int *)malloc(local_size * sizeof(int));

    local_v3 = (int *)malloc(local_size * sizeof(int));



    // Root process creates an array of random numbers

    if (rank == 0)

    {

        randomVector(v1, size);

        randomVector(v2, size);

    }





    // use scatter to distribute work among all worker processes

    MPI_Scatter(v1, local_size, MPI_INT, local_v1, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(v2, local_size, MPI_INT, local_v2, local_size, MPI_INT, 0, MPI_COMM_WORLD);



    // addition done locally on each process

    for (int i = 0; i < local_size; i++)

    {

        local_v3[i] = local_v1[i] + local_v2[i];

    }



// #1

    // use gather local_v3 result back to the root process : v3

    MPI_Gather(local_v3, local_size, MPI_INT, v3, local_size, MPI_INT, 0, MPI_COMM_WORLD);



// #2

    // use Reduce local_v3 result back to the root process : v3

    //MPI_Reduce(local_v3, v3, local_size, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);



// Record end time 

    double end_time = MPI_Wtime(); // in seconds 

    double execution_time = 1000* (end_time - start_time); // convert to milliseconds



// show total time, not individual for each process

// rank 0 = master

    if(rank == 0)

    {

        cout << "\n * Gather * Total time taken by function: " << execution_time << " milliseconds" << endl;

    }



    // Free allocated memory

    free(v1);

    free(v2);

    free(v3);

    free(local_v1);

    free(local_v2);

    free(local_v3);



    // Finalize the MPI environment

    MPI_Finalize();



    return 0;

}

