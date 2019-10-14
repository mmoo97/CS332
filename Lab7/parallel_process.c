#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

#define LINESIZE 1024

void writeFile(char *dir, char* array[10]){
	int i;

	FILE *f = fopen(dir, "wb");
	
	for(i=0;i<5;i++) { // change for big array
        fprintf(f, "%s", array[i]);
     }

	fclose(f);
}

int main(int argc, char* args[]) {
	char* list_items[10];
	char line[LINESIZE];
	int i, count;

	FILE *fptr = fopen("input.txt", "r");
	if(fptr == NULL){
		printf("Error reading input file input.txt\n");
		exit (-1);
	}

	count = 0;
	while (fgets(line, LINESIZE, fptr) != NULL){
		list_items[count] = strdup(line);
		count++;		
	}

	fclose(fptr);

	time_t currtime;

	time(&currtime);

	printf("%s\n", ctime(&currtime));

	pid_t pid;
    int status;

    for(i = 0; i < 2; i++){ // change this later

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

}


	//writeFile("output.log", list_items);

	
}
