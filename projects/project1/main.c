/*
Name: Mitchell Moore
BlazerId:mmoo97
Project #:1
*/

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int listdir(const char *name, int indent, int s, int sval, int f, char** fval) { // block recycled from stackoverflow from (https://stackoverflow.com/questions/8436841/how-to-recursively-list-directories-in-c-on-linux) 
    DIR *dir; //initalize dir pointer
    struct dirent *entry; //create a directory structure
    int count = 0; 

    if (!(dir = opendir(name))) // exit program if dir doesn't exist/retruns null
        return -1;

    while ((entry = readdir(dir)) != NULL) { // iterate through each filename till none left
        if (entry->d_type == DT_DIR) { // make sure entry is of type dir
            char path[1024]; // create buffer for line output
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
		continue;
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name); // put chars in path array 
            printf("%*s[%s]\n", indent, "", entry->d_name); // print dir name
           count += listdir(path, indent + 2, s, sval, f, fval); // recursively print elements till null
        } else if (entry->d_type == DT_LNK) {
	    printf("%*s- %s %s\n", indent, "", entry->d_name, "(symbolic link)");	
	}else {
            printf("%*s- %s\n", indent, "", entry->d_name); // print file name
        }
	count++;
    }
    closedir(dir);
    return count;
}

int main(int argc, char** argv){
	int i;
	int count = 0;
	int opt; 
    char *dir;

    int s = 0;
    int sval = 0;
    int f = 0;
    char * fval;


	while(optind < argc) { 
	       if ((opt = getopt(argc, argv, ":s:f:h")) != -1) {  
             switch(opt) {  
                 case 's':
                    if (dir == NULL) {
                        printf("***Current Directory***\n");
                        dir = ".";
                    }
                    s = 1;
                    sval = atoi(optarg);
    
                     break;  
                 case 'f':   
                     if (dir == NULL) {
                        printf("***Current Directory***\n");
                        dir = ".";
                    }
                    f = 1;
                    fval = optarg;

                     break;  
                 case 'h':   
                    printf("\ncommand <directory> [<options>] \n");
                    printf("\t-s <file size in bytes>\n\t\tList all files with file size greater than or equal to the value specified.");
                    printf("\n\t-f <string pattern>\n\t\tList all files that contain specified substring pattern.\n\n");
                     break;  
                 case ':':   
                     printf("option -s or -f missing a value.\n");
                     return -1;  
                 case '?':   
                     printf("Unrecognised option: %c\n", optopt);
                     return -1;  
             } 
         }else {
            dir = argv[1];
            optind++;
         }
	} 
	if (argc == 1){ 
		printf("***Current Directory***\n");
		count = listdir(".", 0, 0, 0, 0, &fval);
	}else {
        count = listdir(dir, 0, s, sval, f, &fval);
    }
    
	printf("( %d ITEMS TRAVERSED. ) \n", count);
    //printf("\ns = %d; sval = %d; f = %d; fval = %s\n", s,sval,f,fval); //test args

	return 0;
}
