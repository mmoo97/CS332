#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static void sig_usr(int signo) {
	switch(signo) {
	   case SIGINT: 
		printf("\nchild received SIGINT signal %d\n", signo);
		break;
	   case SIGQUIT:
		printf("\nchild received SIGQUIT signal %d\n", signo);
		break;
	   case SIGUSR1:
		printf("\nchild received SIGUSR1 signal %d\n", signo);
		break;
	   case SIGUSR2:
		printf("\nchild received SIGUSR2 signal %d\n", signo);
		break;
	   case SIGTSTP:
		printf("\nchild received SIGTSTP signal %d\n", signo);
		break;
	   default:
		printf("\nchild received signal %d\n", signo);
	}
}


int main(int argc, char **argv) {

	signal(SIGINT, sig_usr);
    signal(SIGTSTP, sig_usr);

	sleep(10);

	return 0;
}