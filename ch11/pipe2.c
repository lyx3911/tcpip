#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

/* 单个管道 双向通信 */

int main()
{
    int fds[2];
    char str1[] = "who are you?";
    char str2[] = "thank you";
    char buf[BUF_SIZE];

    pipe(fds);
    pid_t pid = fork(); //注意要先创建管道再fork进程

    if(pid==0){
        write(fds[1],str1,sizeof(str1));
        sleep(2);
        read(fds[0],buf,BUF_SIZE);
        printf("father proc read message: %s\n", str1);
    }
    else{
        read(fds[0],buf,BUF_SIZE);
        printf("child proc read message: %s\n", str2);
        write(fds[1],str2,sizeof(str2));
        sleep(3);
    }
}