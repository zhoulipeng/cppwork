#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[] = "Hello Master HaKu!";

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("sock() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    clnt_addr_size = sizeof(clnt_addr);
    do{
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1)
            error_handling("accept() error");
        write(clnt_sock, message, sizeof(message));
        int z;
        int error=0;
        socklen_t len = 4;
        sleep(1);
        //recv(int sockfd, void *buf, size_t len, int flags);
        //char buf = 0;
        //z = recv(clnt_sock, &buf, 0, 0);
        
	//fprintf(stderr, "getsock opt return %d, error = %d, errno = %d\n", z, buf, errno);
	// 这种方法在srs 中有时能检测出close wait， 有时检测不出来，
	// 但是在本程序中一次也没检测出来
        z = getsockopt(clnt_sock, SOL_SOCKET, SO_ERROR, &error, &len);
        fprintf(stderr, "getsock opt return %d, error = %d, errno = %d, len = %d\n", z, error, errno, len);
        
    }while(1);

    close(clnt_sock);
    close(serv_sock);

    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
