#include<stdio.h>
/*
Name: Mitchell Moore
BlazerId:mmoo97
Project #:1
*/

int main(int argv, char** argc){
	int i;
	if (argv == 1) {
		printf("Tracing route....\n");
		//Todo: Directory list from current dir
	}else {
		for (i = 0; i < argv; i++)
		printf("Arg %d: %s\n", i, argc[i]);
	}
	return 0;
}

