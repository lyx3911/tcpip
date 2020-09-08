#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 30

void error_handling(char *message);

int main()
{
    struct sockaddr_in recv_adr;
    int str_len, state;

    int recv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
    recv_adr.sin_port = htons(atoi("9190"));

    if( connect(recv_sock,(struct sockaddr*)&recv_adr,sizeof(recv_adr))==-1 )
        error_handling("connect error!");
    
    write(recv_sock,"hello world",strlen("hello world"));
    
    close(recv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}