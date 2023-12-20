#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
/*
filename: myrm.c
description: remove files on argv[1~]
input: char *argv[]
output: none
*/
	int		i;

	if (argc == 1)  {
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		exit(1);
	}

	for (i = 1 ; i < argc ; i++)  {
		if (remove(argv[i]) < 0/* remove */) { //remove file on argv[1~]
			perror("remove");
			exit(1);
		}
	}

	return 0;
}
