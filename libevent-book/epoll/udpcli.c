// http://www.cnblogs.com/lidan/archive/2012/05/04/2482877.html
// 根据这篇博文 ，为什么
// udp也要调用connect函数呢，为什么udp的connect是两次握手呢
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MAXLINE 80
#define SERV_PORT 8888
void do_cli(FILE *fp, int sockfd, struct sockaddr *pservaddr, socklen_t servlen)
{
    int n;
    char sendline[MAXLINE], recvline[MAXLINE + 1];
    if(connect(sockfd, (struct sockaddr *)pservaddr, servlen) == -1)
    {
        perror("connect error");
        exit(1);
    }   
    while(fgets(sendline, MAXLINE, fp) != NULL)
    {
        write(sockfd, sendline, strlen(sendline));
        n = read(sockfd, recvline, MAXLINE);
        if(n == -1)
        {
            perror("read error");
            exit(1);
        }
        recvline[n] = 0;    
        fputs(recvline, stdout);
    }
}
int main(int argc, char **argv)
{
int sockfd;
struct sockaddr_in servaddr;
if(argc != 2)
{
    printf("usage: udpclient <IPaddress>\n");
    exit(1);
}
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
    {
        printf("[%s] is not a valid IPaddress\n", argv[1]);
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    do_cli(stdin, sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    return 0;
}
