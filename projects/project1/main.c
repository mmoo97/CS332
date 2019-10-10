/*
Name: Mitchell Moore
BlazerId:mmoo97
Project #:1
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <find.h>

int main(int argc, char** argv){
	int i;
	int count = 0;
	int opt; 
    char *dir;

    int s = 0;
    int sval = 0;
    int f = 0;
    char * fval = "";


	while(optind < argc) { 
	       if ((opt = getopt(argc, argv, ":s:f:h")) != -1) {  // getopt condition to take in args (letter is option, ':' is arg)
            switch(opt) {  
                 case 's':
                    if (dir == NULL) {
                        printf("***Current Directory***\n"); // if directory not supplied then assume current directory
                        dir = ".";
                    }
                    s = 1; // set to show that s option selected it was provided
                    sval = atoi(optarg); // pass on the option arg to store
    
                     break;  
                 case 'f':   
                     if (dir == NULL) {
                        printf("***Current Directory***\n"); // if directory not supplied then assume current directory
                        dir = ".";
                    }
                    f = 1; // set to show that f option selected it was provided
                    fval = optarg; // pass on the option arg to store

                     break;  
                 case 'h':   // help menu if -h option is used
                    printf("\ncommand <directory> [<options>] \n");
                    printf("\t-s <file size in bytes>\n\t\tList all files with file size greater than or equal to the value specified.");
                    printf("\n\t-f <string pattern>\n\t\tList all files that contain specified substring pattern.\n\n");
                     break;  
                 case ':':   // case where option is dselected without giving corresponding arg
                     printf("option -s or -f missing a value.\n");
                     return -1;  
                 case '?':   // case when any other option is passed
                     printf("Unrecognised option: %c\n", optopt);
                     return -1;  
             } 
         }else {
            dir = argv[1]; // case where program is run without args
            optind++;
         }
	} 
	if (argc == 1){ 
		printf("***Current Directory***\n");
		count = listdir(".", 0, 0, 0, 0, fval); // run program without args assuming current dir
	}else {
        count = listdir(dir, 0, s, sval, f, fval);
    }
    
	printf("( %d ITEMS TRAVERSED. ) \n", count); // pass on items traversed

    // printf("\ns = %d; sval = %d; f = %d; fval = %s\n", s,sval,f,fval); //test args output

	return 0;
}
