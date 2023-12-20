#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

#define	MAX_PATH	256

void JustDoIt(char *path) {
/*
filename: mylsr.c(call by reference)
description: print file names in current directory and call bottom directories by reference
input: char *path
output: none
*/
	DIR				*dp;
	struct dirent	*dep;
	struct stat		statbuf;
	char			fullpath[MAX_PATH];
	if ((dp = opendir(path)) == NULL)  { // check the directory
		perror("opendir");
		exit(0);
	}

	printf("\n%s:\n", path);
	while (dep = readdir(dp))  { // ignore "." and ".."
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		printf("%s\n", dep->d_name);
	}
	
	rewinddir(dp);	/* rewind */ //goes to first directory 
	while (dep = readdir(dp))  {
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		strcpy(fullpath, path);
		strcat(fullpath, "/");
		strcat(fullpath, dep->d_name);
		if (lstat(fullpath,&statbuf)<0/* lstat */)  {
			perror("lstat");
			exit(1);
		}
		if (S_ISDIR(statbuf.st_mode))  { //current directory 
			JustDoIt(fullpath);
		}
	}
	
	closedir(dp);
}

int main() {
	//call function JustDoIt
	JustDoIt(".");

	return 0;
}
