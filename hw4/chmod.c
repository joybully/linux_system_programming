#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(){
/*
filename: chmod.c
description: change mode of bar
input: none
output: none
*/
	struct stat	statbuf;

	if (stat("bar", &statbuf) < 0)  { //case of no information of bar 
		perror("stat");
		exit(1);
	}

	if (chmod("bar", (statbuf.st_mode & ~S_IWGRP) | S_ISUID) < 0) {  //make bit for 1 of bar user_id 
		perror("chmod");
		exit(1);
	}

	/* S_IRUSR, S_IWUSR, S_IRGRP, S_IROTH : ON */
	if (chmod("foo",S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0 /* chmod */)  {
		perror("chmod");
		exit(1);
	}
}
