#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void main(void)
{
    int fd = open("test1.txt", O_WRONLY | O_CREAT, 0777);
    printf("salut\n");
    printf("salut\n");
    printf("salut\n");
    printf("salut\n");
    printf("salut\n");

    dup2(fd, STDOUT_FILENO);
    // close(fd);
}