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

void listdir(const char *name, int indent)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            printf("%*s[%s]\n", indent, "", entry->d_name);
            listdir(path, indent + 2);
        } else {
            printf("%*s- %s\n", indent, "", entry->d_name);
        }
    }
    closedir(dir);
}

int main(int argv, char** argc){
	int i;
/*	if (argv == 1) {
		printf("Tracing route....\n");
		//Todo: Directory list from current dir
	}else {
		for (i = 0; i < argv; i++)
		printf("Arg %d: %s\n", i, argc[i]);
	}*/
	if (argv == 1){
		printf("Current Directory\n");
		listdir(".", 0);
	}else{
		listdir(argc[1], 0);
	}
	return 0;
}

