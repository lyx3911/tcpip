#include <stdio.h>
#include <fcntl.h>

int main(void)
{
    FILE *fp;
    int fd = open("data.dat",O_WRONLY|O_CREAT|O_TRUNC);

    if(fd == -1) {
        printf("file open error! \n");
        return -1;
    }

    printf("First file descriptor: %d \n", fd);

    fp = fdopen(fd, "w");
    fputs("Network C programming \n",fp);

    printf("Second file descriptor: %d \n", fileno(fp));
    
    fclose(fp);
    return 0;
}