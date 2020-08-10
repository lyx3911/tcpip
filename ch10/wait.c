#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    pid_t pid = fork();
    if(pid==0)//子进程
    {
        puts("Hi, I'm child pid");
    }
    else
    {
        printf("Child proc ID: %d\n",pid);
    }

    if(pid==0)
    {
        puts("end child pid");
        sleep(20);
        exit(1); //子进程终止
    }
    else
    {
        puts("end parent pid\n");
        int status;
        // wait(&status); //会阻塞直到子进程终止

        while(!waitpid(-1,&status,WNOHANG)) //不会阻塞
        {
            sleep(3);
            puts("sleep 3 sec...");
        }

        if(WIFEXITED(status))
            printf("Child send: %d \n",WEXITSTATUS(status));
        sleep(10);
    }
    
    
    return 0;
}