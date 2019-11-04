/* Simple program to illustrate the use of fork-exec-wait pattern. 
 *  * To Compile: gcc -Wall forkexecv.c
 *   * To Run: ./a.out
 *    */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    pid_t pid;
    int status;
    char *args[] = {"uname", "-a", (char *)NULL};

    pid = fork();
    if (pid == 0) { /* this is child process */
        execv("/usr/bin/uname", args);
        printf("If you see this statement then execl failed ;-(\n");
	exit(-1);
    } else if (pid > 0) { /* this is the parent process */
        printf("Wait for the child process to terminate\n");
        wait(&status); /* wait for the child process to terminate */
        if (WIFEXITED(status)) { /* child process terminated normally */
            printf("Child process exited with status = %d\n", WEXITSTATUS(status));
        } else { /* child process did not terminate normally */
            printf("Child process did not terminate normally!\n");
            /* look at the man page for wait (man 2 wait) to determine
 *                how the child process was terminated */
        }
    } else { /* we have an error */
        perror("fork"); /* use perror to print the system error message */
        exit(EXIT_FAILURE);
    }

    printf("[%ld]: Exiting program .....\n", (long)getpid());

    return 0;
}

