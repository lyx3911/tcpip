#include <stdio.h>
#include <pthread.h>

void * thread_main(void *arg);

int main(int argc, char *argv[])
{
    pthread_t t_id;
    int pthread_param = 5;
    void * thr_ret;

    if(pthread_create(&t_id, NULL, thread_main, (void*)&pthread_param)!=0){
        puts(" pthread_create error! ");
        return -1;
    }
    
    if(pthread_join(t_id, &thr_ret)!=0){
        puts(" pthread_join error! ");
        return -1;
    }

    puts("end of main");
    free(thr_ret);
    return 0;
}

void * thread_main(void *arg){
    int cnt = *((int *)arg);
    for(int i=0;i<cnt;i++)
    {
        sleep(1);
        printf("running thread, cnt = %d \n", i);
    }
    return NULL;
}