/* Fall 2019 CS 332 - Solution to Lab 4 
 * To compile: gcc -Wall -o lab4 lab4_solution.c
 * To run: ./lab4 input.txt output.txt
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

/* Given a character buffer with string separated by newline
 * this function updates the array of pointers with appropriate addresses
 */
int createarray(char *buf, long bufsize, char **array) {
	int i, count;

	for (i = 0, array[0] = &buf[0], count = 1; i < bufsize; i++) {
		if (buf[i] == '\n') {
		   buf[i] = '\0';
		   array[count++] = &buf[i+1];
		}
	}

	return count-1;
}

/* Uses insertion sort to sort an array of pointers to strings */
void sort(char **array, int count) {
	char *temp;
	int i, currLoc;

	for (i=1; i<count; i++) {
	    currLoc = i;
	    while (currLoc > 0 && strcmp(array[currLoc-1],array[currLoc]) > 0) {
	        temp = array[currLoc];
		array[currLoc] = array[currLoc-1];
		array[currLoc-1] = temp;
		currLoc--;
	    }
	}
}

/* Uses binary search to search a string in an array of strings */
int search(char **array, char *key, int start, int end) {
	int mid, value; 

	if (start > end) 
	   return -1;

	mid = (start + end)/2;
	if ((value = strcmp(array[mid], key)) == 0) 
		return mid;
	else if (value > 0) 
		return search(array, key, start, mid-1);
	else
	  	return search(array, key, mid+1, end); 
}

/* Open a file with the given filename, seek to the specified offset,
 * write the string provided, and close the file */
void updatefile(char *filename, int offset, char *str) {
	int fdout;

	if ((fdout = open(filename, O_RDWR)) == -1) {
		printf("Error opening file %s\n", filename);
		exit (-1);
	}
	if (lseek(fdout, offset, SEEK_SET) >= 0) { 
	   if (write(fdout, str, strlen(str)) != strlen(str)) 
	      printf("Error while writing replace string to file\n"); 
	} else 
	   printf("Lseek error while writing to file\n"); 

	close(fdout); 
}

int main(int argc, char *argv[]) {
	int  fdin, fdout, i, n, count, index, offset;
	struct stat statbuf;
	char *buf;
	char **array;
	char key[BUFSIZ];
	
	if (argc != 3){
		printf("Usage: %s <infile> <outfile>\n", argv[0]);
		exit (-1);
	}
	
	if ((fdin = open(argv[1], O_RDONLY)) == -1) {
		printf("Error opening file %s\n", argv[1]);
		exit (-1);
	}

	/* Since we don't know the number of elements in the file,
	   let's determine the filesize and allocate that many elements.
	   We will also allocate a buffer of that size to read the entire
	   file in one read. */
	if (lstat(argv[1], &statbuf) < 0) {
		printf("Unable to lstat file %s\n", argv[1]);
		exit (-1);
	}
	off_t filesize = statbuf.st_size;
	buf = malloc(sizeof(char)*filesize);
	array = malloc(sizeof(char *)*filesize);

	if ((n = read(fdin, buf, filesize)) < 0) {
		printf("Error reading input file\n");
                exit (-1);
	}
	close(fdin);

	count = createarray(buf, filesize, array);

#ifdef DEBUG
	for (i = 0; i < count; i++) 
	    printf("%s\n", array[i]);
#endif

	sort(array, count);

#ifdef DEBUG
	for (i = 0; i < count; i++) 
	    printf("%s\n", array[i]);
#endif

	/* write sorted data to file */
	if ((fdout = open(argv[2], O_CREAT|O_WRONLY|O_TRUNC, 0700)) == -1) {
		printf("Error opening file %s\n", argv[2]);
		exit (-1);
	}
	for (i=0; i<count; i++) {
	    write(fdout, array[i], strlen(array[i]));
	    write(fdout, "\n", 1);
	}
	close(fdout);

	printf("Enter the search string: ");
	scanf("%s",key);

	if ((index = search(array, key, 0, count)) != -1) 
	    printf("Key %s found at position %d\n", key, index);
	else {
	    printf("Key %s not found\n", key); 
	    exit(-1);
	}

	printf("Enter the new string to replace '%s': ", key);
	scanf("%s", key);

	/* compute the file offset to seek */
	for (i=0, offset=0; i<index; i++)
	    offset += strlen(array[i]) + 1;

	updatefile(argv[2], offset, key);

	free(array);
	free(buf);

	return 0;
}

