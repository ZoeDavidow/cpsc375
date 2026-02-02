// Zoe Davidow
// CPSC 375 HW 3
// learning FIFOs/named pipes
// process A

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main() {

    mkfifo("FIFO1", 0666);
    int fdw = open("FIFO1", O_WRONLY);
    write(fdw, "this is my message\n", 20);
    close(fdw);

    char buffer[100];
    int fdr = open("FIFO2", O_RDONLY);
    //read(fdr, buffer, 100);
    //printf("Data received: %s\n", buffer);
    int chars;
    read(fdr, &chars, sizeof(int));
    printf("character count: %d\n", chars);

    int words;
    read(fdr, &words, sizeof(int));
    printf("word count: %d\n", words);

    int lines;
    read(fdr, &lines, sizeof(int));
    printf("line count: %d\n", lines);
    
    close(fdr);

    return 0;
}