#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h> 

int mutex = 0;
int lock = 0;
int unlock = 1;
 
static int count = 0;
void *test_func(void *arg)
{
        int i = 0;
        for(i = 0; i < 10000000; i++)
        {
            while (!(__sync_bool_compare_and_swap (&mutex,lock, 1) )) usleep(10000);// usleep = 10000效率最高，原来是100000
            count++;
            __sync_bool_compare_and_swap (&mutex, unlock, 0);

        }

        return NULL;

}

 

int main(int argc, const char *argv[])
{
    struct timeval tv1, tv2;
    gettimeofday(&tv1, NULL);
    printf("main %ld,%ld\n", tv1.tv_sec, tv1.tv_usec);
    pthread_t thread_ids[2];
    int i = 0;
 
    for(i = 0; i < sizeof(thread_ids)/sizeof(pthread_t); i++)
    {
            pthread_create(&thread_ids[i], NULL, test_func, NULL);
    }
 

    for(i = 0; i < sizeof(thread_ids)/sizeof(pthread_t); i++)
    {
            pthread_join(thread_ids[i], NULL);

    }
 

    gettimeofday(&tv2, NULL);
    printf("main %ld,%ld, count = %d\n", tv2.tv_sec, tv2.tv_usec, count);

    return 0;

}
