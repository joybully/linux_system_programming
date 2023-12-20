#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
/*
filename: myrd.c
description: remove directory argv[1]
input: char *argv[]
output: none 
*/
	if (argc != 2)  {
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}

	if (rmdir(argv[1]) < 0/* rmdir */) { // remove directory 
		perror("rmdir");
		exit(1);
	}

	return 0;
}
