#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex;

void * thread_inc(void * arg);
void * thread_des(void * arg);

int sum = 0;

int main()
{
    pthread_t t_id1, t_id2;

    pthread_mutex_init(&mutex, NULL);

    pthread_create( &t_id1, NULL, thread_inc, NULL);
    pthread_create( &t_id2, NULL, thread_des, NULL);

    pthread_join(t_id1,NULL);
    pthread_join(t_id2,NULL);

    printf("result: sum = %d \n", sum);
    pthread_mutex_destroy(&mutex);
    return 0;
}

void * thread_inc(void *arg)
{
    pthread_mutex_lock(&mutex);
    sum ++;
    printf("sum = %d \n", sum);
    sleep(2);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void * thread_des(void *arg)
{
    pthread_mutex_lock(&mutex);
    sum --;
    printf("sum = %d \n", sum);
    sleep(2);
    pthread_mutex_unlock(&mutex);
    return NULL;
}