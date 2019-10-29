/*
Resources: 
		https://brennan.io/2015/01/16/write-a-shell-in-c/
		https://www.geeksforgeeks.org/making-linux-shell-c/
		http://www.bagill.com/ascii-sig.php
*/

#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 

#define BUFFSIZE 64 
#define TOKEN_DELIM " \t\r\n\a" // delim args

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

int list(char **args);
int cd(char **args);
int help(char **args);
int quit(char **args);
int history(char **args);
 

void startupMessage() { //welcome message when entering shell 
    clear();
    printf("Welcome to"); 
    printf("\n\n ____  _                    ____  _          _ _ \n"); 
    printf("| __ )| | __ _ _______ _ __/ ___|| |__   ___| | |\n"); 
    printf("|  _ \\| |/ _` |_  / _ \\ '__\\___ \\| '_ \\ / _ \\ | |\n"); 
    printf("| |_) | | (_| |/ /  __/ |   ___) | | | |  __/ | |\n");
    printf("|____/|_|\\__,_/___\\___|_|  |____/|_| |_|\\___|_|_|\n");
    char* username = getenv("USER"); 
    printf("\n\nUser:\t%s\n\n\n", username); 
    sleep(2); 
   
} 

char *blazersh_read_line(void) {
  char *line = NULL;
  size_t bufsize = 0; 
  getline(&line, &bufsize, stdin);
  return line;
}

char **blazersh_split_line(char *line) { // parse line into array of args
  int bufsize = BUFFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "blazersh: allocation error\n");
    exit(-1);
  }

  token = strtok(line, TOKEN_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += BUFFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "blazersh: allocation error\n");
        exit(-1);
      }
    }

    token = strtok(NULL, TOKEN_DELIM);
  }
  tokens[position] = NULL; // make the last postion null to terminate
  return tokens;
}

int blazersh_launch(char **args) { // execute regular camand line progs with args in new process
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("blazersh");
    }
    exit(-1);
  } else if (pid < 0) {
    // Error forking
    perror("blazersh");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int main(int argc, char **argv) {

	startupMessage();
	char* arg = blazersh_read_line();
	char** split_args = blazersh_split_line(arg);
	//printf("\n\ninput: %s\n", split_args[0]);

  blazersh_launch(split_args);

	return 0;
}
