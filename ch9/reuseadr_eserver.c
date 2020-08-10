#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

void error_handling(char *message);

int main()
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t len;

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    //2-设置time-wait属性，使在time-wait时间内，套接字也可以分配给其他程序
    socklen_t opt_len;
    int option;
    opt_len = sizeof(option);
    option = true;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option,opt_len);


    memset(&serv_addr,0,sizeof(serv_addr));
    char *ip = "127.0.0.1";
    char *port = "9190";
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(atoi(port));

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
        error_handling("bind error");

    printf("wait for connect...\n");
    
    listen(serv_sock, 1);

    len = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&len);
    if(clnt_sock==-1)
        error_handling("accept error");
    sleep(30);//停顿方便观察

    /*1. 经测试发现默认状态下，服务器程序被强制终止后，三分钟内重启都是bind error*/

    write(clnt_sock,"hello",6); 

    close(clnt_sock);
    close(serv_sock);

    return 0;
}

void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}