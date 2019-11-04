#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

#define LINESIZE 1024

char * temp[2];

void split(char* array) {
	int count = 0;
	int init_size = strlen(array);
	char delim[] = " ";

	char *ptr = strtok(array, delim);

	while (ptr != NULL)
	{
		temp[count] = strdup(ptr);
		ptr = strtok(NULL, delim);
		count++;
	}
}

void writeFile(char *dir, char* array){
	int i;

	FILE *f = fopen(dir, "wb");
	
    fprintf(f, "%s", array); 

	fclose(f);
}

int main(int argc, char* argv[]) {
	char* list_items[10];
	char* base_items[10];
	char line[LINESIZE];
	int i, count;

	char new_str[1000];

	FILE *fptr = fopen("input.txt", "r");
	if(fptr == NULL){
		printf("Error reading input file input.txt\n");
		exit (-1);
	}

	count = 0;
	while (fgets(line, LINESIZE, fptr) != NULL){
		list_items[count] = strdup(line);
		base_items[count] = strdup(line);
		count++;		
	}

	fclose(fptr);

	time_t start;
	time_t finish;

	// printf("%s\n", ctime(&start));
    
    for(i = 0; i < 3; i++){ // change this later

		pid_t pid;
	    int status;

	    split(list_items[i]);

	    pid = fork();
	    if (pid == 0) { 
	        execl(temp[0], temp[1], (char *)NULL);
	        printf("If you see this statement then execl failed ;-(\n");
		exit(-1);
	    } else if (pid > 0) { 
	        printf("Wait for the child process to terminate\n");
	        time(&start);
	        sleep(2);
	        wait(&status); 
	        time(&finish);
	        
			strcat(new_str, base_items[i]);
			strcat(new_str, " ");
			strcat(new_str, ctime(&start));
			strcat(new_str, " ");
			strcat(new_str, ctime(&finish));
			{
				
			};

	        if (WIFEXITED(status)) { 
	            printf("Child process exited with status = %d\n", WEXITSTATUS(status));
	        } else { 
	            printf("Child process did not terminate normally!\n");
	           
	        }
	    } else {
	        perror("fork"); 
	        exit(EXIT_FAILURE);
	    }

}

	writeFile("output.log", new_str);

	
}
