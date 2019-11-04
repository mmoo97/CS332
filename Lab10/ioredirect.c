/* Simple program to illustrate the implementation of I/O redirection.
 * This version uses execvp and command-line arguments to create a new process.
 * To Compile: gcc -Wall -o ioredirect ioredirect.c
 * To Run: ./ioredirect <command> [args]
 * The new child process created executes the program <command> by reading 
 * input from the file stdin.txt and writes output to the file stdout.txt.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    pid_t pid;
    int status;
    int fdin, fdout;

    /* display program usage if arguments are missing */
    if (argc < 2) {
        printf("Usage: %s <command> [args]\n", argv[0]);
        exit(-1);
    }

    /* open file to read standard input stream,
       make sure the file stdin.txt exists, even if it is empty */
    if ((fdin = open("stdin.txt", O_RDONLY)) == -1) {
       printf("Error opening file stdin.txt for input\n");
       exit(-1);
    }

    /* open file to write standard output stream in append mode.
       create a new file if the file does not exist. */
    if ((fdout = open("stdout.txt", O_CREAT | O_APPEND | O_WRONLY, 0755)) == -1) {
       printf("Error opening file stdout.txt for output\n");
       exit(-1);
    }

    pid = fork();
    if (pid == 0) { /* this is child process */
        /* replace standard input stream with the file stdin.txt */
        dup2(fdin, 0);

        /* replace standard output stream with the file stdout.txt */
        dup2(fdout, 1);

        execvp(argv[1], &argv[1]);
        /* since stdout is written to stdout.txt and not the terminal,
           we should write to stderr in case exec fails, we use perror
           that writes the error message to stderr */
	perror("exec");
	exit(-1);
    } else if (pid > 0) { /* this is the parent process */
        /* output from the parent process still goes to stdout :-) */
        printf("Wait for the child process to terminate\n");
        wait(&status); /* wait for the child process to terminate */
        if (WIFEXITED(status)) { /* child process terminated normally */
            printf("Child process exited with status = %d\n", WEXITSTATUS(status));
            /* parent process still has the file handle to stdout.txt, 
               now that the child process is done, let us write to
               the file stdout.txt using the write system call */
            write(fdout, "Hey! This is the parent process\n", 32);
            close(fdout);
            /* since we opened the file in append mode, the above text 
               will be added after the output from the child process */
        } else { /* child process did not terminate normally */
            printf("Child process did not terminate normally!\n");
            /* look at the man page for wait (man 2 wait) to determine
               how the child process was terminated */
        }
    } else { /* we have an error */
        perror("fork"); /* use perror to print the system error message */
        exit(EXIT_FAILURE);
    }

    return 0;
}
