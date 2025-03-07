#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

pthread_mutex_t task_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  available = PTHREAD_COND_INITIALIZER;
static int work = 0;
static int die = 0;

typedef struct node {
    struct node * next;
} node_t;

node_t * head = NULL;

void add_task()
{
    //pthread_mutex_lock(&task_lock);
    node_t * new_node = malloc(sizeof(node_t));
    new_node->next = NULL;
    printf("Head @%p\n", head);
    printf("New task created at %p\n", new_node);
    if(NULL == head) {
        head = new_node;
    } else {
        node_t * temp = head;
        node_t * prev = NULL;
        while(temp)
        {
            // traverse through the list 
            prev = temp;
            temp = temp->next;
        }
        printf("Adding new node after %p\n", prev);
        prev->next = new_node;
    }
    //pthread_mutex_unlock(&task_lock);
}

void remove_task()
{
    node_t * temp = head;
    printf("Head at %p\n", head);
    if(head != NULL)
    {
        head = head->next;
        if(NULL != temp) {
            printf("Removing task @%p\n", temp);
            free(temp);
        }
    }
}

void * thread_function()
{
    int ret = 0;
    while(1) 
    {
        ret = pthread_mutex_lock(&task_lock);
        if(ret != 0) {
            printf("pthread_mutex_lock failed\n");
        }
        
        while(head == NULL && !die) {
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
        if(head != NULL) {
            remove_task(); // do the work
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
    for(i=0; i < 4; ++i)
    {
        add_task();
    }
    
    sleep(5); // wait 
    printf("work remaining\n");
    pthread_cond_broadcast(&available);
    pthread_mutex_unlock(&task_lock);

    sleep(5);

    pthread_mutex_lock(&task_lock);
    if(head == NULL) {
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
