//http://blog.csdn.net/im3094/article/details/1627676
/*
linux三种加锁的方式比较:
记录锁
信号量锁
pthread_mutex_lock

*/
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void test_filelock(int locktimes)
{
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

        struct flock lock, unlock;
        int count = 0;
        int fd;

        lock.l_type = F_WRLCK;
        lock.l_start = 0;
        lock.l_whence = SEEK_SET;
        lock.l_len = 1;

        unlock.l_type = F_ULOCK;
        unlock.l_start = 0;
        unlock.l_whence = SEEK_SET;
        unlock.l_len = 1;

        fd = open("lock.test", O_CREAT);

        for (count = 0; count < locktimes; ++count){
                fcntl(fd, F_SETLKW, &lock);
                fcntl(fd, F_SETLKW, &unlock);
        }
}

void test_pthread_lock(int locktimes)
{
        int i;
        for (i = 0; i < locktimes; ++i){
                pthread_mutex_lock(&mutex);
                pthread_mutex_unlock(&mutex);
        }
}

void test_semlock(int locktimes)
{
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>

        struct sembuf bufLock, bufUnlock;
        int iCount = 0;

        int semid = semget(0x33332222, 1, IPC_CREAT|0666);

        if (semid == -1){
                printf("semget error/n");
                return ;
        }

        semctl(semid, 0, SETVAL, 1);

        bufLock.sem_num = 0;
        bufLock.sem_op = -1;
        bufLock.sem_flg = SEM_UNDO;

        bufUnlock.sem_num = 0;
        bufUnlock.sem_op = 1;
        bufUnlock.sem_flg = SEM_UNDO;

        for ( iCount = 0; iCount < 10000000; ++iCount){
                semop(semid, &bufLock, 1);
                semop(semid, &bufUnlock, 1);
        }
}

int main(int argc, char **argv)
{
        if(argc != 3){
                printf("usage: test_lock file|sem|pthread locktimes/n");
                exit(1);
        }

        int locktimes = atoi(argv[2]);
        if(strcmp(argv[1], "file") == 0)
                test_filelock(locktimes);
        else if(strcmp(argv[1], "sem") == 0)
                test_semlock(locktimes);
        else
                test_pthread_lock(locktimes);

        return 0;
}
