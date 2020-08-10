//利用信号处理消灭僵尸进程

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

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

int main()
{
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD,&act,0);

    pid_t pid = fork();

    if(pid==0) //子进程
    {
        puts("Hi, I'm child proc");
        sleep(12);
        return 12;
    }
    else //父进程
    {
        puts("Hi I'm parent proc");
        printf("Child proc id: %d\n",pid);
        for(int i=0;i<5;i++)
        {
            puts("wait...");
            sleep(5);
        }
    }
    return 0;
    
}