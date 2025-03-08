#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t customer_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t available = PTHREAD_COND_INITIALIZER;
static int stock = 0;
int offer_ends = 0;

void * thread_function(void *arg)
{
    int ret = 0;
    while(1)
    {
        ret = pthread_mutex_lock(&customer_lock);
        if(ret != 0)
        {
            printf("pthread_mutex_lock() failed\n");
            exit(1);
        }
        
        while(stock == 0 && offer_ends != 1)
        {
            printf("Customer [%ld] joining the waiting queue\n", pthread_self());
            ret = pthread_cond_wait(&available, &customer_lock);
            if (ret != 0)
            {
                printf("pthread_cond_wait() failed\n");
            }
        }

        if(stock) 
        {
            --stock;
            printf("Customer ID: %ld: Hurray!! Won discounted offer\n", pthread_self());
        } 

        if (offer_ends) {
            printf("Customer ID: %ld: Discounted price offer already closed\n", pthread_self());
            pthread_mutex_unlock(&customer_lock);
            break;
        }
        pthread_mutex_unlock(&customer_lock);
    }

    printf("Customer [%ld] returning main() home\n", pthread_self());
    return NULL;
}

int main(int argc, char **argv)
{
    int i = 0;
    int ret = 0;

    if (argc < 2)
    {
        printf("Invalid usage\n;");
        printf("Launch the program as below:\n");
        printf("\t./pthread_question_14 <num customers>\n");
        exit(1);
    }

    int num_customers = atoi(argv[1]);
    pthread_t threads[num_customers];

    printf("NUmber of customers chosen by the user = %d\n", num_customers);

    for(int i=0; i < num_customers; ++i)
    {
        ret = pthread_create(&threads[i], NULL, thread_function, NULL);
        if(ret != 0)
        {
            printf("pthread_create failed!\n");
            exit(1);
        }


    }

    sleep(2);
    ret = pthread_mutex_lock(&customer_lock);
    if(ret != 0)
    {
        printf("pthread_mutex_lock() failed\n");
        exit(1);
    }

    stock = 10; // 10 discounted price offer
    ret = pthread_cond_broadcast(&available);
    if(ret != 0)
    {
        printf("pthread_mutex_broadcast() failed\n");
        exit(1);
    }
    ret = pthread_mutex_unlock(&customer_lock);
    if(ret != 0)
    {
        printf("pthread_customer_unlock() failed\n");
    }

    sleep(5);

    ret = pthread_mutex_lock(&customer_lock);
    if(ret != 0)
    {
        printf("pthread_mutex_lock() failed\n");
    }

    if(stock == 0) {
        printf("Looks like all 10 offers gone pretty quickly!\n");
        offer_ends = 1;
        ret = pthread_cond_broadcast(&available);
        if(ret != 0)
        {
            printf("pthread_mutext_broadcast() failed\n");
        }
    }
    ret = pthread_mutex_unlock(&customer_lock);
    if(ret != 0)
    {
        printf("pthread_mutex_unlock() failed\n");
    }

    for(i=0; i < num_customers; ++i)
    {
        ret = pthread_join(threads[i], NULL);
        if(ret != 0)
        {
            printf("pthread_join() failed\n");
            exit(1);
        }
    }

    return 0;
}
