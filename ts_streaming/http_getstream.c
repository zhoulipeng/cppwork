#include<iostream>  
#include<unistd.h>
#include<arpa/inet.h>  
#include<sys/socket.h>  
#include<cstdlib>  
#include<cstdio>  
#include<cstring>  
  
using namespace std;  
  
#define HTTP_RESPONSE_MAX_LEN  512  
int main(void)  
{  

    char *http_msg = "GET /stream HTTP/1.1\r\n"
    "Host: 192.168.100.109:10086\r\n"
    "User-Agent: VLC/2.2.4 LibVLC/2.2.4\r\n"
    "Range: bytes=0-\r\n"
    "Connection: close\r\n"
    "Icy-MetaData: 1\r\n\r\n";
    printf("http_msg lenth = %d\n", strlen(http_msg));
    int fd=0,res=0,len=0;
    unsigned int server_port = 10086;
    const char *server_ip = "192.168.100.109";
    unsigned char sendbuf[512]="", getbuf[1024]="";
    unsigned char http_line_tail_tag[2] = {0x0d, 0x0a};
    unsigned char mpeg2ts_start_tag[3] = {0x0d, 0x0a, 0x47};
    unsigned char mpeg2ts188 = { 0 };
    struct sockaddr_in server;
      
    //init server sockaddr
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server.sin_addr.s_addr);
      
    //create tcp socket
    fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd<0)
    {
        perror("socket");
        exit(-1);
    }
    cout<<"create socket success,fd:"<<fd<<endl;
      
    //connect to server...
    res = connect(fd,(struct sockaddr *)&server,sizeof(server));
    if(res!=0)
    {
        perror("connect");
        close(fd);
        exit(-1);
    }
    cout<<"connect to server success."<<endl;
    send(fd, http_msg, strlen(http_msg), 0);//send data to server 
    // 找到http回应信息，找到最后一个\r\n,并找到ts流 0x47标志 
    len = recv(fd, getbuf, sizeof(getbuf), 0);
    int sum_len = len;
    while(len <= HTTP_RESPONSE_MAX_LEN){
        len = recv(fd, getbuf + len, sizeof(getbuf) - sum_len, 0);
        sum_len += len;
    }while(1);
    printf("111 len = %d,%2hx\n", sum_len, getbuf[0]);
    int i = 0;
    for(i = 0; i < sum_len; i++)
    {
        printf("%02hx", getbuf[i]);
    }
    // 有可能读到一个残缺的 188 ts包, 计算残缺值
    printf("\n");
    while(1)
    {
        //cout<<">:";
        //cin.getline(sendbuf,sizeof(sendbuf));
        //send(fd,sendbuf,strlen(sendbuf),0);//send data to server
        //TODO recv 只管把读取到的数据方到缓冲区中，只要缓冲区大于
        //188就可以从缓冲区取 mpeg2ts 数据包
        len = recv(fd, mpeg2ts188, sizeof(mpeg2ts188),0);//receive data from server
        usleep(100);
        for(i = 0; i < len; i++)

        //getbuf[len]='\0';
        printf("len = %d,%2hx\n", len, getbuf[0]);
        if(len != 188)
        {
            cout<<"server disconnected"<<endl;
            close(fd);
            break;
        }
        //cout<<"recv from server:"<<len<<endl;
    }
    close(fd);
    return 0;
}
