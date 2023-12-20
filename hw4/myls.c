#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>

int main() {
/*
filename: myls.c
description: print file names in current directory
input: none
output: none
*/
	DIR				*dp; // get the directory
	struct dirent	*dep;//there are names and i-nodes
	if ((dp = opendir(".")) == NULL)  {
		perror("opendir");
		exit(0);
	}

	while (dep = readdir(dp)/* readdir */)  { //read dir and get the i-nodes
		printf("%s   ", dep->d_name);
	}
	
	closedir(dp);
	return 0;
}
