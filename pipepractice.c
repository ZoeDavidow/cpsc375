#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

void main() {
    int pipefd[2];
    char buff[100];

    if (pipe(pipefd) < 0) // create a pipe
        perror("pipe error");
    
    printf("readfd = %d, writefd = %d\n", pipefd[0], pipefd[1]);
}