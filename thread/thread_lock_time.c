#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
static int value = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int getValue()
{
	pthread_mutex_lock(&mutex);
	value++;
	pthread_mutex_unlock(&mutex);
	return value;
}
void *work_thread1(void *p)
{
	struct timeval tv1, tv2;
        gettimeofday(&tv1, NULL);
        printf("thread1start %ld,%ld\n", tv1.tv_sec, tv1.tv_usec);

        int i = 0;
        for(i = 0; i < 10000000; i++)
        {
		pthread_mutex_lock(&mutex);
        	value++;
        	pthread_mutex_unlock(&mutex);

        }
        gettimeofday(&tv2, NULL);
        printf("thread1end %ld,%ld value = %d\n", tv2.tv_sec, tv2.tv_usec, value);

	return NULL;
}

void *work_thread2(void *p)
{
	struct timeval tv1, tv2;
        gettimeofday(&tv1, NULL);
	printf("thread2start %ld,%ld\n", tv1.tv_sec, tv1.tv_usec);

        int i = 0;
        for(i = 0; i < 10000000; i++)
        {
		pthread_mutex_lock(&mutex);
        	value++;
        	pthread_mutex_unlock(&mutex);

        }
	gettimeofday(&tv2, NULL);
	printf("thread2end %ld,%ld value = %d\n", tv2.tv_sec, tv2.tv_usec, value);
        return NULL;
}
int main(int argc, char *argv[])
{
	pthread_t pid1, pid2;
	struct timeval tv1, tv2;
	gettimeofday(&tv1, NULL);
	printf("main %ld,%ld\n", tv1.tv_sec, tv1.tv_usec);
	pthread_create(&pid1, NULL, work_thread1, NULL);
	pthread_create(&pid2, NULL, work_thread2, NULL);
	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);
	gettimeofday(&tv2, NULL);
	printf("main %ld,%ld\n", tv2.tv_sec, tv2.tv_usec);
	
}
