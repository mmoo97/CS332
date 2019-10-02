#include <stdio.h> 
#include <string.h> 

int main(int argc, char** argv) { 
    char student1[] = "Alice\0"; 
    char student2[] = "Bob\0"; 
    char student3[] = "bob\0"; 
    char student4[] = "Alice\0"; 

    printf("strcmp(student1, student2) = %d\n", strcmp(student1, student2)); 
    printf("strcmp(student2, student1) = %d\n", strcmp(student2, student1)); 
    printf("strcmp(student2, student3) = %d\n", strcmp(student2, student3)); 
    printf("strcmp(student1, student4) = %d\n", strcmp(student1, student4)); 

    return 0; 
} 
