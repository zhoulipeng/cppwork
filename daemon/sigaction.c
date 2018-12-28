#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

/*
 * SIGQUIT ctrl + /
 * SIGHUB  terminal close
 * SIGINT  ctrl + c
 * SIGTERM default kill PID
 * SIGUSR1 kill -USR1
 */
static void sig_usr(int signum)
{
    if (signum == SIGUSR1) {
        printf("SIGUSR1 received\n");
    } else if (signum == SIGUSR2) {
        printf("SIGUSR2 received\n");
    } else {
        printf("signal %d received\n", signum);
    }
}

int main(void)
{
    char buf[512];
    int n;
    struct sigaction sa_usr;
    sa_usr.sa_flags = 0;
    sa_usr.sa_handler = sig_usr; //信号处理函数

    sigaction(SIGQUIT, &sa_usr, NULL);
    // 如果捕获SIGHUP信号, 关闭终端，会让程序进入后台，父进程号变为1
    sigaction(SIGHUP, &sa_usr, NULL);
    sigaction(SIGINT, &sa_usr, NULL);
    sigaction(SIGTERM, &sa_usr, NULL);
    sigaction(SIGUSR1, &sa_usr, NULL);
    sigaction(SIGUSR2, &sa_usr, NULL);

    printf("My PID is %d\n", getpid());

    while (1) {
        if ((n = read(STDIN_FILENO, buf, 511)) == -1) {
            if (errno == EINTR) {
                printf("read is interrupted by signal\n");
            }
        } else {
            buf[n] = '\0';
            printf("%d bytes read: %s\n", n, buf);
        }
    }

    return 0;
}
