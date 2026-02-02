// Zoe Davidow
// CPSC 375 HW 3
// learning FIFOs/named pipes
// process B

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main() {

    char buffer[100];
    int fdr = open("FIFO1", O_RDONLY);
    read(fdr, buffer, 100);
    //printf("Data received: %s\n", buffer);
    close(fdr);

    // calculate character, word, line count

    int chars = 0;
    int i = 0;
    while(buffer[i]) {
        chars++;
        i++;
    }

    int words = 0;
    int j = 0;
    while (buffer[j]) {
        if ((buffer[j] == ' ' )|| (buffer[j] == '\n')) {
            words++;
        }
        j++;
    }

    int lines = 0;
    int k = 0;
    while (buffer[k]) {
        if ((buffer[k] == '\n') || (buffer[k] == EOF)) {
            lines++;
        }
        k++;
    }

    mkfifo("FIFO2", 0666);
    int fdw = open("FIFO2", O_WRONLY);
    //write(fdw, "character count: ", 18);
    write(fdw, &chars, sizeof(int));
    write(fdw, &words, sizeof(int));
    write(fdw, &lines, sizeof(int));
    close(fdw);
    return 0;
}