#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t customer_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t available = PTHREAD_COND_INITIALIZER;
static int stock = 10;

void * thread_fucntion(void *arg)
{
    return NULL;
}

int main(int argc, char **argv)
{
    int i = 0;

    if (argc < 2)
    {
        printf("Invalid usage\n;");
        printf("Launch the program as below:\n");
        printf("\t./pthread_question_14 <num customers>\n");
        exit(1);
    }

    int num_customers = atoi(argv[1]);
    printf("NUmber of customers chosen by the user = %d\n", num_customers);

    for(int i=0; i < num_customers; ++i)
    {

    }



}
