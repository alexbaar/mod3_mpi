#include <mpi.h>

#include <stdio.h>

#include <string.h>







int main(int argc, char** argv) {

    int numtasks, rank, name_len, tag=1;

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



//                  ACTIVITY 1 PART 1

/*

    // if the rank is 0 it means it is master

    if (rank == 0) {

        char msg[msgLen] = "Hello World!";



        for (int i = 1; i < numtasks; i++) {

            MPI_Send(msg, strlen(msg)+1, MPI_CHAR, i, tag, MPI_COMM_WORLD);

            printf("Master %d sending message: %s to worker: %d\n", rank, msg, i);

        }

    }

    // worker processes have rank from 1 to the number of tasks –1. 

        else {

        char msg[msgLen];



        // Receive message from the master process

        MPI_Recv(msg, msgLen, MPI_CHAR, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);



        printf("Process %d received message: %s\n", rank, msg);

    }

    */



   //                  ACTIVITY 1 PART 2





    char msg[msgLen] = "Hello World!";

   //  if the rank is 0 it means it is master

    if (rank == 0) {

        // Master process broadcasts the message to all workers

        MPI_Bcast(msg, msgLen, MPI_CHAR, 0, MPI_COMM_WORLD);

    } else {

        // Worker processes receive the broadcasted message

        MPI_Bcast(msg, msgLen, MPI_CHAR, 0, MPI_COMM_WORLD);

        printf("Process %d received message: %s\n", rank, msg);

    }



    // Finalize the MPI environment

    MPI_Finalize();

}