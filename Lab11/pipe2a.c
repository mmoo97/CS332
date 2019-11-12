/* Simple program to illustrate how to implement pipes.
 * This example creates a pipe and then creates two child processes.
 * The first child replaces the stdout with write end of the pipe and
 * execs the first command. The second child replaces the stdin with 
 * read end of the pipe and execs the second command. Thus, the stdout
 * of the first child is sent to the stdin of the second child. The parent
 * process closes both ends of the pipe and waits for the child process
 * to terminate.
 *
 * This version uses popen and pclose instead of pipe, fork/exec/wait, and
 * dup2 system calls.
 *
 * To Compile: gcc -Wall pipe2a.c
 * To Run: ./a.out <command1> <command2> 
 *          Output of <command1> will be the input for <command2>
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    FILE *fp1, *fp2;
    char line[BUFSIZ];

    if (argc != 3) {
        printf("Usage: %s <command1> <command2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* create a pipe, fork/exec command argv[1], in "read" mode */
    /* read mode - parent process reads stdout of child process */
    if ((fp1 = popen(argv[1], "r")) == NULL) {
	perror("popen");
	exit(EXIT_FAILURE);
    }

    /* create a pipe, fork/exec command argv[2], in "write" mode */
    /* write mode - parent process writes to stdin of child process */
    if ((fp2 = popen(argv[2], "w")) == NULL) {
	perror("popen");
	exit(EXIT_FAILURE);
    }

    /* read stdout from child process 1 and write to stdin of 
       child process 2 */
    while (fgets(line, BUFSIZ, fp1) != NULL) {
	if (fputs(line, fp2) == EOF) {
	   printf("Error writing to pipe\n");
	   exit(EXIT_FAILURE);
        }
    }

    /* wait for child process to terminate */
    if ((pclose(fp1) == -1) || pclose(fp2) == -1) {
	perror("pclose");
	exit(EXIT_FAILURE);
    }

    return 0;
}
