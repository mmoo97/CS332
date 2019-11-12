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
#include <signal.h>

#define BUFFSIZE 64 
#define TOKEN_DELIM " \t\r\n\a" // delim args
#define LINESIZE 1024

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

int list(char **args);
int cd(char **args);
int help(char **args);
int quit(char **args);
int history(char **args);
int jobs(char **args);
int cont(char **args);

char *builtin_str[] = {
  "list",
  "cd",
  "help",
  "history",
  "quit",
  "jobs",
  "cont"
};

int (*builtin_func[]) (char **) = {
  &list,
  &cd,
  &help,
  &history,
  &quit,
  &jobs,
  &cont
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
    char* pwd = getenv("PWD");
    printf("\n\nUser:\t%s\n\n\n", username); 

    sleep(1); 
   
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

int blazersh_launch(char **args, char *line) { // execute regular camand line progs with args in new process
  pid_t pid, wpid;
  int status;
  FILE *fp1, *fp2;
  char output[1024];

  if ((fp1 = popen(line, "r")) == NULL) {
    perror("popen");
    exit(EXIT_FAILURE);
  }

  fgets(output, 1024, fp1);
  printf("%s\n", output);

    /* create a pipe, fork/exec command argv[2], in "write" mode */
    /* write mode - parent process writes to stdin of child process */
  // if ((fp2 = popen(argv[2], "w")) == NULL) {
  //   perror("popen");
  //   exit(EXIT_FAILURE);
  // }

  // pid = fork(); 
  // if (pid == 0) {
  //   // Child process
  //   if (fp1 == -1) {
  //     perror("blazersh");
  //   }
  //   exit(-1);
  // } else if (pid > 0){
  //   // Parent process

  //   signal(SIGINT, SIG_IGN);
  //   signal(SIGTSTP, SIG_IGN);

  //   wpid = waitpid(pid, &status, WUNTRACED);
        
  //   if (WIFEXITED(status)) { /* child process terminated normally */
  //       //printf("Child process exited with status = %d\n", WEXITSTATUS(status));
  //   } else if(WIFSTOPPED(status)) {
  //       printf("Process with pid=%d has been STOPPED\n", pid);
  //   } else if(WTERMSIG(status) == 2) {
  //       printf("Process with pid=%d has been INTERRUPTED...\n", wpid);
  //   } else { /* child process did not terminate normally */
  //       printf("Child process did not terminate normally!\n");
        
  //       /* look at the man page for wait (man 2 wait) to determine
  //          how the child process was terminated */
  //   }
  // } else {
  //   // Error forking
  //   perror("blazersh");
  // } 
  

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
    printf("%s\n", getenv("PWD"));
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            printf("%s\t", dir->d_name);
        }
        closedir(d);
        printf("\n");
    }
    return 1;

}

int cd(char **args) { 
  if (args[1] == NULL) { // if no directory given throw expected format
    fprintf(stderr, "blazersh: expected argument to \"cd\"\n");
    // todo: could implement usage of home environment variable
  } else {
    if (chdir(args[1]) != 0) { // cd to directory given
      perror("blazersh"); // throw error if dir doesn't exist
    }else {
      printf("%s\n", getenv("PWD"));
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
  // char line[1024];
  // int count;
  // FILE *fptr = fopen("blazersh.log", "r");
  // if(fptr == NULL){
  //   printf("Error reading input file listings.csv\n");
  //   exit (-1);
  // }

  // count = 0;
  // while (fgets(line, LINESIZE, fptr) != NULL){
  //   printf("%s\n", line);;
  // }

  // fclose(fptr);

  // return 1;
  
  printf("Not yet implemented :(\n");
  return 1;
}

int jobs(char ** args) {
  
  FILE *fp;
  char path[1035];

  /* Open the command for reading. */
  
  if ((fp = popen("ps", "r")) == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }

  /* Read the output a line at a time - output it. */
  while (fgets(path, sizeof(path), fp) != NULL) {
    printf("%s", path);
  }

  /* close */
  pclose(fp);

  return 1;
}

int cont(char ** args) {
  printf("continue...\n\n");
  int result;
  int pid;
  sscanf(args[1], "%d", &pid);

  if (args[1] == NULL) { // if no pid given throw expected format
    fprintf(stderr, "blazersh: expected argument to \"cont\"\n");
    // todo: could implement usage of home environment variable
  } else if ((result = kill(pid, SIGCONT)) != 0) { // continue process
      perror("blazersh"); // throw error if dir doesn't exist
  }
    if (result == 0)
      printf("Process %s started.\n", args[1]);
    return 1;
  }

int quit(char **args) {
  clear(); // resets the prompt screen
  return 0;
}

int blazersh_execute(char **args, char *line) { // determines if command is internal or not and executes
  int i;

  if (args[0] == NULL) { // If empty command entered
    return 1;
  }

  for (i = 0; i < blazersh_num_builtins(); i++) { 
    if (strcmp(args[0], builtin_str[i]) == 0) { // check if any of the buld in functons called
      return (*builtin_func[i])(args); // execute build in with args
    }
  }

  return blazersh_launch(args, line); // otherwise, execute the non internal commands
}

void blazersh_loop(void) { // creates a command loop until user calls quit function.
  char *line;
  char line2[1024];
  char *current_dir;
  char *temp_dir = malloc(1024 * sizeof(char*));
  char **args;
  int status;
  int line_num = 1;

  current_dir = getenv("PWD");
  strcat(temp_dir, current_dir);
  //free(current_dir);

  FILE *f = fopen(strcat(temp_dir, "/blazersh.log"), "wb");

  do {
    printf("blazersh> "); // show prompt
    line = blazersh_read_line(); // take in line input as single array of chars
    strcat(line2, line);
    fprintf(f, "  %d  %s", line_num, line); // write history to file
    args = blazersh_split_line(line); // split it and tokenize into individual args
    status = blazersh_execute(args, line2); // status code of executed args. zero will end loop. (using quit functon)

    free(line); // clear var
    free(args); // clear var
    
    line_num++;
  
  } while (status); // run function infinitely until return code of 0 is given
  
  free(temp_dir);
  fclose(f);
}

int main(int argc, char **argv) {

	startupMessage(); 
	blazersh_loop(); 

	return 0;
}
