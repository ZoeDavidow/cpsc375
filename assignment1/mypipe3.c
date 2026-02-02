// Zoe Davidow
// CPSC 375 assignment 1
// mypipe3.c
// 1.27.26

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define TOK_LIMIT 20 // define a size for tokenized argument array

/*
    function: tokenize
    purpose: breaks up a string into multiple tokens and puts them in a dynamically allocated array
    parameters: char *string
            will be a command followed by arguments
    returns: a pointer to the array of command and arguments
*/
char **tokenize(char *string) {
    char **args;
    args = (char **) malloc(TOK_LIMIT * sizeof(char *));
    int argcount = 0; // number of arguments

    char *token = strtok(string, " "); // get token
    //loop to parse arguments
    while (token != NULL) {
        args[argcount] = token; // put token in args array
        argcount++; // increase argument count
        token = strtok(NULL, " "); // get token
    }
    args[argcount] = NULL; // null terminator

    return args;
}

/*
    function: main
    purpose: takes three commands from the command line, then uses fork() and pipes to run them
    parameters: 
        int argc: number of arguments in the command line
        char *argv[]: array of strings, holds the arguments given in command line
    returns: an int to stop warnings
*/
int main(int argc, char *argv[]) {

    // argv[1] holds command 1, argv[2] command 2, argv[3] command 3
    
    // 2 file descriptors for 2 pipes
    int fd1[2], fd2[2];

    // create new argument arrays to pass in exec
    // call tokenize()
    char **arg1 = tokenize(argv[1]); 
    char **arg2 = tokenize(argv[2]);
    char **arg3 = tokenize(argv[3]);

    // create pipes and check for error
    if (pipe(fd1) == -1) {
        perror("Pipe");
        exit(1);
    }
    if (pipe(fd2) == -1) {
        perror("Pipe");
        exit(1);
    }

    // call fork() to create first child
    switch(fork()) {

        case -1: //error
            perror("fork");
            exit(2);

        case 0: //child

            switch(fork()) { // second fork, creates grandchild
                case -1: // error
                    perror("fork");
                    exit(2);
                case 0: // grandchild
                    dup2(fd2[0], STDIN_FILENO); // makes STDIN_FILENO refer to the same file description as fd2[0]

                    // close all file descriptors
                    close(fd1[0]);
                    close(fd1[1]);
                    close(fd2[0]);
                    close(fd2[1]);

                    // call exec and check for error
                    if (execvp(arg3[0], arg3)) {
                        perror("exec");
                    }
                    // exit
                    exit(3);

                default: // child, put parent to grandchild
                    dup2(fd1[0], STDIN_FILENO); // when it's acting as the child
                    dup2(fd2[1], STDOUT_FILENO); // when it's acting as the parent

                    // close file descriptors
                    close(fd1[0]);
                    close(fd1[1]);
                    close(fd2[0]);
                    close(fd2[1]);

                    // call exec and check for error
                    if (execvp(arg2[0], arg2)) {
                        perror("exec");
                    }
                    // exit
                    exit(4);     
            }
            
        default: // parent
            dup2(fd1[1], STDOUT_FILENO); // make STDOUT refer to the same file descriptor as fd1[1]
            
            // close all file descriptors
            close(fd1[0]);
            close(fd1[1]);
            close(fd2[0]);
            close(fd2[1]);

            // call exec and check for error
            if (execvp(arg1[0], arg1)) {
                perror("exec");
            }
            // exit
            exit(5);

    }

    return -1;
}