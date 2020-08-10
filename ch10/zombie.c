#include <stdio.h>
#include <unistd.h>

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
        sleep(30);
    }

    if(pid==0)
    {
        puts("end child pid");
    }
    else
    {
        puts("end parent pid\n");
    }
    
    
    return 0;
}