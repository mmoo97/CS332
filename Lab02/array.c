#define size 100
#include <stdlib.h> 
#include <time.h>
#include <stdio.h> 

int main(int args, char** argv) { 
   srand48((unsigned int)time(NULL)); 
   double array [size]; 
   int i;
   for (i=0; i<size; i++) { 
      array[i] = drand48();
      printf("%lf\n", array[i]);
   }
   double min_value = array[0]; 

   for (i=0; i<size; i++) { 
    if (array[i] < min_value) { 
        min_value = array[i]; 
    }
}
   printf("Min value = %If\n", min_value); 
} 
