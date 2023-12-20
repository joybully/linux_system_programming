#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
/*
filename: mymd.c
description: make directory on argv[1]
input: char *argv[]
output: directory argv[1] 
*/
	if (argc != 2)  {
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}

	/* 755 permission */
	if (mkdir(argv[1], 0755) < 0/* mkdir */) { //make directory
		//mode for 755(user-rwx, group,other-r/x)
		perror("mkdir");
		exit(1);
	}

	return 0;
}
