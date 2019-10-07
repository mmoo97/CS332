#include<stdio.h>
/*
Name: Mitchell Moore
BlazerId:mmoo97
Project #:1
*/

int main(int argv, char** argc){
	int i;
	for (i = 0; i < argv; i++)
	printf("Arg %d: %s\n", i, argc[i]);

	return 0;
}

