#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    int i=0;
    fprintf(stderr, "%d\n", getpid());
    sleep(30);
    for(;i<100;i++)
    {
        sleep(1);
        void *p = malloc(1024000);
        //sleep(1);
        printf("printf = %d\n", i);
        if(p)
        {
            free(p);
            p = NULL;
        }
    }
    sleep(200);
}
