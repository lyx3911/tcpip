#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{   
    if(sig==SIGALRM)
        puts("Time out");
    alarm(2); //每隔2秒重复产生信号
}

void keycontrol(int sig)
{
    if(sig==SIGINT)
        puts("ctrl + C pressed");
}

int main()
{
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    alarm(2); //预约两秒后的信号

    for(int i=0;i<3;i++)
    {
        puts("wait...");
        sleep(100);
    }
    return 0;
}