#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define MAXLINE 1080
#define SERV_PORT 8888

struct user_admin {
    long long type;
    char username[64];
    char password[64];
    char address[64];
};

void do_echo(int sockfd, struct sockaddr* pcliaddr, socklen_t clilen)
{
    int n;
    struct user_admin ret = { 0, "addr", "user", "pass" };
    printf("mesg=%s, %s, %s\n", ret.address, ret.username, ret.password);
    socklen_t len;
    // char mesg[MAXLINE];
    struct user_admin mesg;
    for (;;) {
        len = clilen;
        memset(&mesg, 0x00, sizeof(mesg));
        n = recvfrom(sockfd, &mesg, MAXLINE, 0, pcliaddr, &len);
        printf("mesg=%s, %s, %s\n", mesg.address, mesg.username, mesg.password);
        // sleep(92); //　udp 客户端，与外网服务器通信，如果udp服务收到消息，超过一定时间再回复udp客户端，客户端就收不到消息了
        sendto(sockfd, &ret, sizeof(ret), 0, pcliaddr, len);
    }
}

int main(void)
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        perror("bind error");
        exit(1);
    }
    do_echo(sockfd, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
    return 0;
}

