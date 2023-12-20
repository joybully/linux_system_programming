#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
/*
filename: myln.c
description: make new name of argv[1] for argv[2]
input: char *argv[]
output: argv[2] 
*/
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destinaion\n", argv[0]);
		exit(1);
	}

	if (link(argv[1],argv[2]) < 0 /*link */)  {
	//make same file with original
	//dose matter what file would be deleted, another one would be survive
		perror("link");
		exit(1);
	}

	return 0;
}
