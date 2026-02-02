// Zoe Davidow
// CPSC 375 assignment 1
// mypipen.c
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

    char *copy = strdup(string); // copy the pointer so you don't corrupt argv
    char **args;
    args = (char **) malloc(TOK_LIMIT * sizeof(char *));
    int argcount = 0; // number of arguments

    char *token = strtok(copy, " "); // get token
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
    function: mypipe
    purpose: creates pipes, forks, execs, and recursively calls
    parameters:
        argcount: number of arguments
        args: array holding arguments
        index: current location in argument array
    returns: nothing
*/
void mypipe(int argcount, char *args[], int index) {
    
    // base case: for the last argument, exec and exit
    if (index == argcount - 1) {
        char **arg = tokenize(args[index]);
        if (execvp(arg[0], arg)) {
            perror("exec");
            exit(1);
        }
    
        return;
    }

    else {
    // file descriptor
    int fd[2];

    // argument array to pass to exec
    char **arg1 = tokenize(args[index]);
    char **arg2 = tokenize(args[index + 1]);


    // create pipe and check for error
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    // call fork in switch case
    switch(fork()) {

        case -1: // error
            perror("fork");
            exit(2);
        
        case 0: // child
            dup2(fd[0], STDIN_FILENO);

            // close file descriptors
            close(fd[0]);
            close(fd[1]);

            // if this is second to last command, exec
            // call exec and check for error
            if (index + 1 == argcount - 1) { 
                if (execvp(arg2[0], arg2)) {
                    perror("exec");
                    exit(1);
                }
            }

            // recursive call
            mypipe(argcount, args, index + 1);

            // exit
            exit(4);

        default: // parent
            dup2(fd[1], STDOUT_FILENO);

            // close all file descriptors
            close(fd[0]);
            close(fd[1]);

            // call exec and check for error
            if (execvp(arg1[0], arg1)) {
                perror("exec");
                exit(1);
            }
            // exit
            exit(5);

    }
    }

    return;
}

/*
    function: main
    purpose: 
        for one argument it calls exec on the command
        for more than one, it calls the recursive mypipe() function
    parameters:
        argc: number of arguments
        argv: character pointer array holding command line arguments
    returns: an int to get rid of warnings
*/
int main(int argc, char *argv[]) {

    // if there is only one argument, handle the whole fork process here
    if (argc == 2) {

        // create argument array to pass to exec
        char **arg = tokenize(argv[1]);

        // call exec and check for error
        if (execvp(arg[0], arg)) {
            perror("exec");
            exit(1);
        }
        // exit
        exit(1);

    }
    // otherwise, use the recursive function
    else {
        mypipe(argc, argv, 1);
    }

    return -1;
}
