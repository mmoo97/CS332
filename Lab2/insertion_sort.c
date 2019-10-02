#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int main(int args, char** argv) {
	int size;
	printf("Enter the number of array elements: ");
	scanf("%d", &size);
	srand((unsigned int)time(NULL)); 
	int array [size]; 
	int i;
	printf("Here is your UNSORTED array of %d elements:\n", size);
	for (i=0; i<size; i++) { 
		array[i] = rand() % 10000;
		printf("%d, ", array[i]);
		if(i == size-1) {
			printf("\n\n");
		}
	}
	int temp, currLoc;
	for (i=1; i<size; i++) {   
		currLoc = i;
	while (currLoc > 0 && array[currLoc-1] > array[currLoc]) {
		temp = array[currLoc];
		array[currLoc] = array[currLoc-1];
		array[currLoc-1] = temp;
		currLoc--;
	}
	}
	printf("Here is your SORTED array of %d elements:\n", size);
        for (i=0; i<size; i++) {
                printf("%d, ", array[i]);
                if(i == size-1) {
                        printf("\n");
                }
}
}
