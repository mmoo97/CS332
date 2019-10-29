/*
Resources: 
		https://brennan.io/2015/01/16/write-a-shell-in-c/
		https://www.geeksforgeeks.org/making-linux-shell-c/
		http://www.bagill.com/ascii-sig.php
    https://www.sanfoundry.com/c-program-list-files-directory/
*/

/*
Name: Mitchell Moore
BlazerId:mmoo97
Project: 2
*/

#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include <dirent.h>

#define BUFFSIZE 64 
#define TOKEN_DELIM " \t\r\n\a" // delim args

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

int list(char **args);
int cd(char **args);
int help(char **args);
int quit(char **args);
int history(char **args);

char *builtin_str[] = {
  "list",
  "cd",
  "help",
  "history",
  "quit"
};

int (*builtin_func[]) (char **) = {
  &list,
  &cd,
  &help,
  &history,
  &quit
};
 

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

int blazersh_num_builtins() { // simple function to give number of possible builtins
  return sizeof(builtin_str) / sizeof(char *);
}

int list(char **args) {

    DIR *d;
    struct dirent *dir;
    d = opendir(getenv("PWD")); // open current directory
    // todo: could implement list to take directory arg instead of implicit dir
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            printf("%s\t", dir->d_name);
        }
        closedir(d);
        printf("\n");
    }
    return(0);

}

int cd(char **args) { 
  if (args[1] == NULL) { // if no directory given throw expected format
    fprintf(stderr, "blazersh: expected argument to \"cd\"\n");
    // todo: could implement usage of home environment variable
  } else {
    if (chdir(args[1]) != 0) { // cd to directory given
      perror("blazersh"); // throw error if dir doesn't exist
    }
  }
  return 1;
}

int help(char **args) { // gives user usage info
  int i;
  printf("BlazerShell\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < blazersh_num_builtins(); i++) { // print every optional internal command
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n"); // should operate as normal
  return 1;
}

int history(char ** args) {

  // todo simply print out file with line numbers

}

int quit(char **args) {
  clear();
  return 0;
}

int blazersh_execute(char **args) { // determines if command is internal or not and executes
  int i;

  if (args[0] == NULL) { // If empty command entered
    return 1;
  }

  for (i = 0; i < blazersh_num_builtins(); i++) { 
    if (strcmp(args[0], builtin_str[i]) == 0) { // check if any of the buld in functons called
      return (*builtin_func[i])(args); // execute build in with args
    }
  }

  return blazersh_launch(args); // otherwise, execute the non internal commands
}

int main(int argc, char **argv) {

	startupMessage();
	char* arg = blazersh_read_line();
	char** split_args = blazersh_split_line(arg);
  list(split_args);
	//printf("\n\ninput: %s\n", split_args[0]);



	return 0;
}
