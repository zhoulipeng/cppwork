#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#define MAXLINE 10
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT 5555
#define INFTIM 1000
#define MAX_EVENT 10000

void setnonblocking(int sock)
{
    int opts;
    opts=fcntl(sock,F_GETFL);
    if(opts<0)
    {
        perror("fcntl(sock,GETFL)");
        exit(1);
    }

    opts = opts|O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts)<0)
    {
        perror("fcntl(sock,SETFL,opts)");
        exit(1);
    }

}

int main(int argc, char *argv[])
{
    int udp_caller_sk = 0;
    int numbytes = 0;
    if ((udp_caller_sk = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
	perror("socket");
	exit(1);
    }
    struct sockaddr_in caller_addr;
    memset(&caller_addr, 0x00, sizeof(caller_addr)); /* 其余部分设成0 */
    caller_addr.sin_family = AF_INET;
    caller_addr.sin_port = htons(5555); /* short, NBO */
    caller_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    char buf[8] = { 0 };
    sprintf(buf, "hello%s", argv[1]);
    printf("%s\n", buf);
    numbytes = sendto(udp_caller_sk, buf, strlen(buf), 0, (struct sockaddr *)&caller_addr, sizeof(caller_addr));
    return 0;
}
