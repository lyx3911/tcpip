/*
注意实验的结果：
    MSG_OOB传递数据时智能返回一个字节
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>

#define BUF_SIZE 30
void error_handling(char *message);
void urg_handler(int signo);

int acpt_sock, recv_sock;

int main()
{
    struct sockaddr_in recv_adr, send_adr;
    int str_len, state;
    char buf[BUF_SIZE];
    socklen_t serv_adr_sz;

    struct sigaction act;
    act.sa_handler=urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGURG,&act, 0);

    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
    recv_adr.sin_port = htons(atoi("9190"));

    if( bind(acpt_sock,(struct sockaddr*)&recv_adr, sizeof(recv_adr))==-1 )
        error_handling("bind error!");
    
    if( listen(acpt_sock,5)==-1 )
        error_handling("listen error!");
    
    serv_adr_sz = sizeof(send_adr);
    recv_sock = accept(acpt_sock, (struct sockaddr*)&recv_adr, &serv_adr_sz);

    fcntl(recv_sock, F_SETOWN, getpid());

    while( (str_len=recv(recv_sock,buf,sizeof(buf),0))!=0 )
    {
        if(str_len==-1) continue;
        buf[str_len] = 0;
        puts(buf);
        putc("\n");
    }
    close(recv_sock);
    close(acpt_sock);
    return 0;

}

void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

void urg_handler(int signo)
{
    int str_len;
    char buf[BUF_SIZE];
    str_len = recv(recv_sock,buf,BUF_SIZE,MSG_OOB);
    buf[str_len] = 0;
    printf("Urgent message: %s\n", buf);
}