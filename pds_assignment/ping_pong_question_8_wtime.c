#include <stdio.h>
#include <mpi.h> 
#include <string.h>
#include <time.h>

#define TOTAL_ITERATIONS 10000
/*
 * 
int MPI_Send(const void *buf, 
             int count, 
             MPI_Datatype datatype, 
             int dest, 
             int tag, 
             MPI_Comm comm)

int MPI_Recv(void *buf, 
             int count, 
             MPI_Datatype datatype, 
             int source, 
             int tag,
             MPI_Comm comm, 
             MPI_Status * status)
*/

int main(int argc, char ** argv)
{
    int comm_size = 0;
    int comm_rank = 0;
    char send_message[5] = {0};
    char recv_message[5] = {0};
    MPI_Status status;
    int i = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    if(comm_rank == 0)
    {
        double start = MPI_Wtime();
        strncpy(send_message, "ping", 4);

        for(i=0; i < TOTAL_ITERATIONS; ++i)
        {
            memset(recv_message, 0, sizeof(recv_message));
            MPI_Send(send_message, 
                     strlen(send_message),
                     MPI_CHAR,
                     1,
                     0,
                     MPI_COMM_WORLD);

            MPI_Recv(recv_message, 
                     4,
                     MPI_CHAR,
                     MPI_ANY_SOURCE,
                     99,
                     MPI_COMM_WORLD,
                     &status);
            printf("Received from source %d, tag %d\n", status.MPI_SOURCE, status.MPI_TAG);
            printf("Received %s\n", recv_message);
        }
        double time_taken = MPI_Wtime() - start;
        printf("Total Time taken for %d iterations = %f seconds\n", TOTAL_ITERATIONS, (double)time_taken);
        printf("Average Time taken for message passing = %f seconds\n", (double)time_taken/TOTAL_ITERATIONS);
    } else {
        
        strncpy(send_message, "pong", 4);

        for(i=0; i < TOTAL_ITERATIONS; ++i)
        {
            memset(recv_message, 0, sizeof(recv_message));
            MPI_Recv(recv_message, 
                     4, 
                     MPI_CHAR,
                     MPI_ANY_SOURCE,
                     MPI_ANY_TAG,
                     MPI_COMM_WORLD,
                     &status);
            printf("source = %d tag = %d\n", status.MPI_SOURCE, status.MPI_TAG);
            printf("received = %s\n", recv_message);
         
            MPI_Send(send_message,
                     strlen("pong"),
                     MPI_CHAR, 
                     0, 
                     99, 
                     MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();

    return 0;
}
