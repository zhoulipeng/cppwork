#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
typedef struct sockaddr SA;
#define MAXLINE 5000
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT 5000
#define INFTIM 1000
int
main(int argc, char **argv)
{
    int                    sockfd;
    char                recvline[MAXLINE + 1];
    struct sockaddr_in    servaddr;

    if (argc != 2){

        printf("usage: a.out <IPaddress>");
	exit(0);
    }
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
	printf("socket error");
	exit(0);
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(5000);   
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
    {
        printf("inet_pton error for %s", argv[1]);
        exit(0);
    }
	char op[16] = { 0 };
	inet_ntop(AF_INET, &servaddr.sin_addr, op, sizeof(op));
	printf("argv[1] = %s, op = %s\n", argv[1], op);
    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
    {    
        printf("connect error");
        exit(1);
    }

    char input[100];
    while (fgets(input, 100, stdin) != EOF)
    {
        write(sockfd, input, strlen(input));

        int n = 0;
        int count = 0;
        while (1)
        {
            n = read(sockfd, recvline + count, MAXLINE);
            if (n == MAXLINE)
            {
                count += n;
                continue;
            }
            else 
                break;
        }
        printf("%s\n", recvline);
    }
    exit(0);
}
