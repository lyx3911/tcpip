#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

#define BUF_SIZE 30 

void read_childproc(int sig);
void error_handling(char* message);

int main()
{
    int serv_sock, clnt_sock;

    //注册信号处理函数
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD,&act,0);

    struct sockaddr_in serv_addr, clnt_addr;
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(atoi("9190"));

    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock==-1)
        error_handling("socket error...");

    if( (bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)))==-1 )
        error_handling("bind error...");

    listen(serv_sock,1);

    while(1)
    {
        socklen_t len = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&len);
        if(clnt_sock==-1)
            continue;
        else
        {
            puts("new client connected...");
        }

        pid_t pid = fork();
        if(pid==-1)
        {
            close(clnt_sock);
            continue;
        }
        if(pid==0)
        {
            close(serv_sock);
            char message[BUF_SIZE];
            while(read(clnt_sock,message,BUF_SIZE)!=0)
            {
                write(clnt_sock,message,BUF_SIZE);
            }
            close(clnt_sock);
            puts("client disconnected...");
        }
        else
        {
            close(clnt_sock);
        }                
    }

    close(serv_sock);

    return 0;
}

void read_childproc(int sig)
{
    int status;
    pid_t id = waitpid(-1,&status,WNOHANG);
    if(WIFEXITED(status))
    {
        printf("Removed proc id: %d\n",id);
        printf("Child send: %d\n",WEXITSTATUS(status));
    }
}
void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
