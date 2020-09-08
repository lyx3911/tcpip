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
    struct sockaddr_in serv_adr, clnt_adr;
    int fds[2];

    pid_t pid;
    struct sigaction act;
    socklen_t adr_sz;
    int str_len, state;
    char buf[BUF_SIZE];
    
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    state = sigaction(SIGCHLD, &act,0);

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0,sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_adr.sin_port = htons(atoi("9190"));
    
    if(bind(serv_sock,(struct sockaddr*)&serv_adr, sizeof(serv_adr) )== -1 ) 
        error_handling("bind socket error!");
    
    if(listen(serv_sock,5)==-1)
        error_handling("listening error!");

    pipe(fds);
    pid = fork();

    if(pid==0){
        FILE * fp = fopen("echomsg.txt","wt");
        char msgbuf[BUF_SIZE];
        int i, len;

        for(i=0;i<10;i++){
            len = read(fds[0],msgbuf,BUF_SIZE);
            fwrite((void*)msgbuf,1,len,fp);
        }

        fclose(fp);
        return 0;
    }

    while(1)
    {
        adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_adr, &adr_sz);

        if(clnt_sock==-1) continue;
        else puts("new client connected ...");

        pid = fork();
        if(pid==0){
            close(serv_sock);
            while((str_len = read(clnt_sock, buf, BUF_SIZE))!=0){
                write(fds[1],buf, str_len); //写给管道 保存文件用
                write(clnt_sock,buf,str_len); //回声
            }
            close(clnt_sock);
            puts("client disconnected ...");
        }
        else 
            close(clnt_sock);
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
