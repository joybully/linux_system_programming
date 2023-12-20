#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
/*
filename: mycd.c
description: print current directory, print directory after move
input: char *argv[]
output: none
*/
	char buf[60];
	if (argc != 2)  {
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}

	getcwd(buf, 60);
	printf("path : %s\n",buf);
	if (chdir(argv[1]) < 0/* chdir */) {  
		perror("chdir");
		exit(1);
	}
	getcwd(buf,60);
	printf("path : %s\n",buf);

	return 0;
}
