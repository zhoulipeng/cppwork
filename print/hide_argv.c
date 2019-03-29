#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

int main(int argc, char *argv[]){
    // strcpy(argv[0], "othername"); // strlen(othername) must less than strlen(all param)
    int argc_count = 0;
    // hide command name and all parameters
    while(1){
        for(size_t j = 0; j < strlen(argv[0]); j++){
            argv[argc_count][j] = ' ';
        }
        ++argc_count;
        if(argc_count == argc) break;
    }

    printf("pid is %ld\n", getpid());
    sleep(108);
    int64_t i;
    struct timeval start;
    gettimeofday(&start, NULL);
    printf("%ld, %ld", start.tv_sec, start.tv_usec);
    for (i = 0; i < 10000000ULL; i++) {
        //getpid(); // state-threads的 getpid可以去掉,提高性能
    }
    gettimeofday(&start, NULL);
    printf("%ld, %ld", start.tv_sec, start.tv_usec);
    return 0;
}

