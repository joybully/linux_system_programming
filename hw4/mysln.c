#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
/*
filename: mysln.c
description: make link argv[2] to go to argv[1]
input: char *argv[]
output: argv[2]
*/
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destinaion\n", argv[0]);
		exit(1);
	}

	if (symlink(argv[1],argv[2])<0 /* symlink */)  {
		//make the link for original file	
		perror("link");
		exit(1);
	}

	return 0;
}
