#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

#define BUF_SIZE 30 

void error_handling(char * message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

void read_routine(int sock)
{
    char buf[BUF_SIZE];
    while( read(sock, buf, BUF_SIZE)!=0 )
        printf("recv from serv: %s\n", buf);
}

void write_routine(int sock, char* buf)
{
    write(sock, buf, BUF_SIZE);
}

int main()
{
    int sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t len;

    if( (sock = socket(PF_INET,SOCK_STREAM,0))==-1 )
        error_handling("sock error...");
    
    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_adr.sin_port = htons(atoi("9190"));

    if( connect(sock,(struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1 )
        error_handling("connect error...");
    else puts("connected to server...");

    pid_t pid = fork();

    while(1)
    {
        if(pid==0)//子进程负责读
            read_routine(sock);
        else //父进程负责写
        {
            printf("Insert message: ");
            char buf[BUF_SIZE];
            scanf("%s",buf);
            if(!strcmp(buf,"q\n")) break;
            write_routine(sock,buf);
        }
        
    }
    close(sock);

    return 0;
}