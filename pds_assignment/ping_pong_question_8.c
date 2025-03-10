#include <stdio.h>
#include <mpi.h> 
#include <string.h>
#include <time.h>
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

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    if(comm_rank == 0)
    {
        clock_t start = clock();
        strncpy(send_message, "ping", 4);

        MPI_Ssend(send_message, 
                 strlen(send_message),
                 MPI_CHAR,
                 1,
                 0,
                 MPI_COMM_WORLD);

        MPI_Recv(recv_message, 
                 4,
                 MPI_CHAR,
                 MPI_ANY_SOURCE,
                 MPI_ANY_TAG,
                 MPI_COMM_WORLD,
                 &status);
        clock_t time_taken = clock() - start;
        printf("Received from source %d, tag %d, error %d\n", status.MPI_SOURCE, status.MPI_TAG, status.MPI_ERROR);
        printf("Received %s\n", recv_message);
        printf("Time taken = %f seconds\n", (float)time_taken/CLOCKS_PER_SEC);
    } else {
        
        MPI_Recv(recv_message, 
                 4, 
                 MPI_CHAR,
                 MPI_ANY_SOURCE,
                 MPI_ANY_TAG,
                 MPI_COMM_WORLD,
                 &status);
        printf("source = %d tag = %d error = %d\n", status.MPI_SOURCE, status.MPI_TAG, status.MPI_ERROR);
        printf("received = %s\n", recv_message);
        
        strncpy(send_message, "pong", 4);
        MPI_Send(send_message,
                 strlen("pong"),
                 MPI_CHAR, 
                 0, 
                 99, 
                 MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
