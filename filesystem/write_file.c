/*
 * open write 等系统调用写文件
 * 将内存数据写到文件中，考虑各种异常
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

struct MemoryStruct{
    char *memory;
    size_t size;
};

int main(int argc, char *argv[])
{
    struct MemoryStruct chunk;
    chunk.size = 10240000;
    chunk.memory = malloc(chunk.size);
    memset(chunk.memory, 'c', chunk.size/2);
    memset(chunk.memory + chunk.size/2, 'd', chunk.size/2);
    int fd = open("file.txt", O_WRONLY|O_CREAT, 0777);
    if(fd == -1){
        printf("failed open\n");
        exit(EXIT_FAILURE);
    }
    ssize_t pos = 0;
    do{
        size_t have_len = chunk.size - pos;
        ssize_t w_len = write(fd, chunk.memory + pos, have_len > 4096?4096:have_len);
        if(w_len == -1){
            printf("failed write %s\n", strerror(errno));
            break;
        }
        pos = pos + w_len;
    }while(chunk.size > pos);
    close(fd);

    return 0;
}
