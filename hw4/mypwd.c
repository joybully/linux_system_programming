#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define	MAX_BUF	256

int main() {
/*
filename: mypwd.c
description: print current working directory on terminal
input: char *argv[]
output: none
*/
	char	buf[MAX_BUF];

	if (getcwd(buf, MAX_BUF) == NULL/* getcwd */) { // get current directory 
		perror("getcwd");
		exit(1);
	}

	printf("%s\n", buf);
	return 0;
}
