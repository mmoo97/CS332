#include <stdio.h> 
#include <string.h> 

int main(int argc, char** argv) { 
    char hello[20] = {'h','e','l','l','o',' ','\0'}; 
    char name[] = "students!\0"; 
    strcat(hello, name); 
    printf("%s\n",hello); 

    return 0; 
} 
