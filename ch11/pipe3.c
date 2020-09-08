#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

/* 两个 双向通信 */

int main()
{
    int fds1[2], fds2[2];
    char str1[] = "who are you?";
    char str2[] = "thank you";
    char buf[BUF_SIZE];

    pipe(fds1);
    pipe(fds2);
    pid_t pid = fork(); //注意要先创建管道再fork进程

    if(pid==0){
        write(fds1[1],str1,sizeof(str1));
        // sleep(2);
        read(fds2[0],buf,BUF_SIZE);
        printf("father proc read message: %s\n", str1);
    }
    else{
        read(fds1[0],buf,BUF_SIZE);
        printf("child proc read message: %s\n", str2);
        write(fds2[1],str2,sizeof(str2));
        // sleep(3);
    }
}