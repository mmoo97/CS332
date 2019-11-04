#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int * genArray(int size);
void printArray(int *arr, int size);
int binarySearch(int *arr, int left, int right, int search);
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
                printf("%d\n", arr[i]);
                if(i == size-1) {
                        printf("\n");
                }
        }
}
int binarySearch(int *arr, int left, int right, int search) 
{ 
	while (left <= right) { 
		int m = left + (right - left) / 2; 
		if (arr[m] == search) 
		    return m; 

		if (arr[m] < search) 
		    left = m + 1;

		else
		    right = m - 1; 
	}

	return -1; 
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

int main(int argc, char** argv) {
	int size, search, result;
	printf("Enter a number of array elements: ");
	scanf("%d", &size);
	int * array = genArray(size);
	sortArray(array, size);
	printArray(array, size);
	
	printf("Enter a number to search for: ");
	scanf("%d", &search);
	
	result = binarySearch(array, 0, size-1, search);
	
	if (result == -1) {
		printf("%d DOESN'T exist in array.\n", search);
	}else {
		printf("%d position in array: %d\n", search, result);
	}

	return 0;
}
