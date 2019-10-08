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

int listdir(const char *name, int indent) { // block recycled from stackoverflow from (https://stackoverflow.com/questions/8436841/how-to-recursively-list-directories-in-c-on-linux) 
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
           count += listdir(path, indent + 2); // recursively print elements till null
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

	while((opt = getopt(argc, argv, ":s:f:")) != -1) {  
             switch(opt) {  
                 case 's':
                     printf("option: %d\n", atoi(optarg));
                     break;  
                 case 'f':   
                     printf("filename: %s\n", optarg);
                     break;  
                 case ':':   
                     printf("option needs a value\n");
                     break;  
                 case '?':   
                     printf("unknown option: %c\n", optopt);
                     break;  
             }
    	}   

	if (argc == 1){
		printf("Current Directory\n");
		count = listdir(".", 0);
	}else if (argc == 2 && strcmp(argv[argc-1],"--help") == 0) {
		printf("\ncommand <directory> [<options>] \n");
		printf("\t-s <file size in bytes>\n\t\tList all files with file size greater than or equal to the value specified.");
		printf("\n\t-f <string pattern>\n\t\tList all files that contain specified substring pattern.\n\n");
	}else {	
		count = listdir(argv[1], 0);
	}
	printf("( %d ITEMS TRAVERSED. ) \n", count);
	
	return 0;
}

