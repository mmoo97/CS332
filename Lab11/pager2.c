/* Simple example to illustrate the use of pipes.
 * This example opens the file provided as the command-line argument,
 * creates a pipe, and forks a child process. Then the parent closes 
 * the read file descriptor of the pipe, reads the contents of the file
 * specified as the command-line argument, writes the contents to the 
 * pipe, and waits for the child process to complete. The child process 
 * closes the write file descriptor of the pipe and execs the "more" 
 * command. This is a simpler version of the program in Figure 15.6 in 
 * the textbook.
 *
 * This version uses popen and pclose instead of pipe/fork/dup2/exec 
 * commands.
 * 
 * To Compile: gcc -Wall pager2.c
 * To Run: ./a.out <filename>
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  FILE *fpin, *fpout;
  char line[BUFSIZ];

  if (argc != 2) {
     printf("Usage: %s <filename>\n", argv[0]);
     exit(-1);
  }

  /* open file for reading */
  if ( (fpin = fopen(argv[1], "r")) == NULL ) {
	printf("Error opening file %s for reading\n", argv[1]);
	exit(-1);
  }

  /* create a pipe, fork/exec process "more", in "write" mode */
  /* write mode - parent process writes, child process reads */
  if ( (fpout = popen("more", "w")) == NULL ) {
      perror("exec");
      exit(EXIT_FAILURE);
  } 

  /* read lines from the file and write it fpout */
  while (fgets(line, BUFSIZ, fpin) != NULL) {
     if (fputs(line, fpout) == EOF) {
        printf("Error writing to pipe\n");
	exit(EXIT_FAILURE);
     }
  }

  /* close the pipe and wait for child process to terminate */
  if (pclose(fpout) == -1) {
     perror("pclose");
     exit(EXIT_FAILURE);
  } 

  exit(EXIT_SUCCESS);
}

