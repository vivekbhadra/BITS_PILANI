#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t task_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  available = PTHREAD_COND_INITIALIZER;
static int work = 0;
static int die = 0;

void * thread_function()
{
    int ret = 0;
    while(1) 
    {
        ret = pthread_mutex_lock(&task_lock);
        if(ret != 0) {
            printf("pthread_mutex_lock failed\n");
        }
        
        while(work == 0 && !die) {
            printf("Thread %ld going to conditional wait\n", pthread_self());
            ret = pthread_cond_wait(&available, &task_lock);
            if (ret != 0) {
                printf("pthread_cond_wait faile\n");
            }
        }

        if(die) {
            pthread_mutex_unlock(&task_lock);
            break;
        }
        if(work) {
            --work; // do the work
            printf("Work Done!\n");
        }

        pthread_mutex_unlock(&task_lock);
    }

    printf("Bye bye cruel world\n");
    return NULL;
}

int main()
{
    pthread_t thread[4];
    int ret = 0;
    int i = 0;

    for(i = 0; i < 4; ++i)
    {
        ret = pthread_create(&thread[i], NULL, thread_function, NULL);
        if(ret != 0)
        {
            printf("pthread_cond_wait");
        }
    }

    pthread_mutex_lock(&task_lock);
    work = 4;
    if(work != 0) {
        printf("work remaining %d \n", work);
        pthread_cond_broadcast(&available);
    }
    pthread_mutex_unlock(&task_lock);

    sleep(5);

    pthread_mutex_lock(&task_lock);
    if(!work) {
        printf("All works done!\n");
        die = 1;
        pthread_cond_broadcast(&available);
    }
    pthread_mutex_unlock(&task_lock);

    for(i=0; i < 4; ++i)
    {
        pthread_join(thread[i], NULL);
    }

    printf("Bye World\n");
    return 0;
}
