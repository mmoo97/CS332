#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void writeFile(char *dir, int lines){
	int i;

	FILE *f = fopen(dir, "wb");
	
	for(i=0;i<lines;i++) { // change for big array
        
     }

	fclose(f);
}

int main(int argc, char* args[]) {
	

}
