#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int * genArray(int size);
void printArray(int *arr, int size);
int * sortArray(int *arr, int size);
 
int * genArray(int size) {
	int i;
	srand((unsigned int)time(NULL));
	int* arr = (int*) calloc(size, sizeof(int)); 	
	for (i=0; i<size; i++) {
		arr[i] = rand() % 10000;
	}
	return (arr);
}

void printArray(int * arr, int size) {
	int i;
        for (i=0; i<size; i++) { 
                printf("%d, ", arr[i]);
                if(i == size-1) {
                        printf("\n\n");
                }
        }
}

int * sortArray(int * arr, int size) {
	
	int temp, currLoc, i;
		for (i=1; i<size; i++) {   
			currLoc = i;
			while (currLoc > 0 && arr[currLoc-1] > arr[currLoc]) {
				temp = arr[currLoc];
				arr[currLoc] = arr[currLoc-1];
				arr[currLoc-1] = temp;
				currLoc--;
			}
		}
	return (arr);	

}


// Takes command line arg for array size
int main(int args, char** argv) {
	int size = atoi(argv[1]);
	int * array =  genArray(size);
	printf("Here is your UNSORTED array of %d elements:\n", size);
	printArray(array, size);
	sortArray(array, size);
	printf("Here is your SORTED array of %d elements:\n", size);
	printArray(array, size);
	free(array);
}

