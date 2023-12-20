#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
/*
filename:mymv.c
description: change file name argv[1] to argv[2]
input: char *argv[]
output: none
*/
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s old new\n", argv[0]);
		exit(1);
	}

	if (rename(argv[1],argv[2])<0/* rename */) { // change name
		perror("rename");
		exit(1);
	}

	return 0;
}
