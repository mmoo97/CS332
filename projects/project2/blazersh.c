#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 

#define MAXLETTERS 1000 
#define MAXCOMMANDS 100

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

void startup_message() 
{ 
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

int main(int argc, char **argv) {
	startup_message(); 
	return 0;
}
