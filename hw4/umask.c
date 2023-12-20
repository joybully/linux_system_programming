#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>	// for use of creat()

int main() {
/*
filename: umask.c
description: limit file mode when creates file
input: none
output: foo, bar
*/
	umask(0);
	//case of creating file give mode : user-read/write, group-read/write, other-read/write
	if (creat("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | 
				S_IROTH | S_IWOTH) < 0)  { //if file creating failed
		perror("creat");
		exit(1);
	}

	/* Set file mode creation mask: S_IRGRP, S_IWGRP, S_IROTH, S_IWOTH */
	// limit mode: group-read/write user-read/write
	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);  /* umask */
	if (creat("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | 
				S_IROTH | S_IWOTH) < 0)  {
		perror("creat");
		exit(1);
	}

	return 0;
}
