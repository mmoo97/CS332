#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define	BUFFSIZE 80000

void print2dArray(char arr[][20], int x, int y);
int binarySearch(char arr[][20], int left, int right, char * search);
void sortArray(char arr[][20], int size);
char * readFile();
void Arrayify(char * arr, int x, int y);
void writeFile(char *dir, char arr[][20]);

char twodee[10000][20];

char * readFile() {

	int  readFileDescriptor;
	long int n;
	static char buf[BUFFSIZE];
	
	readFileDescriptor = open("NameList.csv", O_RDONLY);
	
	if (readFileDescriptor == -1){
		printf("Error with file open\n");
		exit (-1);
	}
	int count = 0;
	while ((n = read(readFileDescriptor, buf, BUFFSIZE)) > 0){
		count++;
		if (count == BUFFSIZE - 1) {
			

		}
	}
	if (n < 0){
		printf("Error reading input file\n");
		exit (-1);
	}

	close(readFileDescriptor);

	return (buf);
}

void Arrayify(char * arr, int x, int y) {
	int i, j, count;

	count = 0;

	for (i = 0; i < x; i++) {
		for (j = 0; j < y; j++) {
			if (arr[count] == '\n'){
				arr[count] = '\0';
				break;
			}
			twodee[i][j] = arr[count];
			count++;
		}
		count++;
	}
}

void print2dArray(char arr[][20], int x, int y) {
        int i, j;

        for (i=0; i < x; i++) {
                printf("%s, ", arr[i]);
            
        }
        printf("\n");
}
int binarySearch(char arr[][20], int left, int right, char * search) 
{ 
	while (left <= right) { 
		int m = left + (right - left) / 2; 
		if (strcmp(arr[m], search) == 0)
		    return m; 

		if (strcmp(arr[m], search) < 0)
		    left = m + 1;

		else
		    right = m - 1; 
	}

	return -1; 
}  


void sortArray(char arr[][20], int size) {

	int i, j;
	char temp[20];

	for(i=0;i<=size;i++)
      for(j=i+1;j<size;j++){
         if(strcmp(arr[i],arr[j])>0){
            strcpy(temp,arr[i]);
            strcpy(arr[i],arr[j]);
            strcpy(arr[j],temp);
         }
      }
}

void writeFile(char *dir, char arr[][20]){
	int i;

	FILE *f = fopen(dir, "wb");
	
	for(i=0;i<10000;i++) {
        fprintf(f,"%s\n",arr[i]);
     }

	fclose(f);


}

int main(int argc, char** argv) {
	char name[20];
	char input[20];
	int i, result, choice;
	Arrayify(readFile(), 10000, 20);
	sortArray(twodee, 10000);
	print2dArray(twodee, 10000, 20);
	printf("Type a name to search for: ");
	scanf("%s", name);
	result = binarySearch(twodee, 0, 10000, name);
	if (result == -1) 
		printf("%s does not exist in array.\n\n", name);
	else
		printf("%s is at Element %d in the array.\n\n", name, result - 1);
	writeFile("names.txt", twodee);

	printf("Enter an elemnt to replace: ");
	scanf("%d", &choice);
	printf("What would you like to change %s to? ", twodee[choice]);
	scanf("%s", input);

	strcpy(twodee[choice], input);
	writeFile("names.txt", twodee);
	printf("\n\n.....Done\n");

	return 0;
}

