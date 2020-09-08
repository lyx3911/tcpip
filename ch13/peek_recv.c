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

int main()
{
    int acpt_sock, recv_sock;
    struct sockaddr_in recv_adr, send_adr;
    int str_len, state;
    char buf[BUF_SIZE];
    socklen_t serv_adr_sz;

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

    while(1){
        str_len = recv(recv_sock, buf, BUF_SIZE, MSG_PEEK|MSG_DONTWAIT); //读取缓冲数据也不删除
        if(strlen>0) //如果没有读到就一直读
            break;
    }

    buf[str_len] = 0;
    printf("Buffering %d bytes: %s \n",str_len, buf);

    str_len = recv(recv_sock,buf,BUF_SIZE,0); //读取缓冲数据但是删除
    if(str_len>=0){
        buf[str_len] = 0;
        printf("read again: %s\n", buf);
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