#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) //make mycp using mmap
{
	int			fdin, fdout;
	char		*src, *dst;
	struct stat	statbuf;

	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if ((fdin = open(argv[1], O_RDONLY)) < 0)  { //open read file 
		perror("open");
		exit(1);
	}
	if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0)  { //open write file 
		perror("open");
		exit(1);
	}

	if (fstat(fdin, &statbuf) < 0)  { 
		perror("fstat");
		exit(1);
	}
	if (lseek(fdout, statbuf.st_size-1, SEEK_SET) < 0)  { //move the file pointer 
		perror("lseek");
		exit(1);
	}
	write(fdout, "", 1); //file size will be same as read file
//there will be only null char in buffer 


	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0))
		== (caddr_t) -1)  {
		perror("mmap");
		exit(1);
	}
	if ((dst = mmap(0, statbuf.st_size, PROT_WRITE, MAP_SHARED, fdout, 0)) 
		== (caddr_t) -1)  {
		perror("mmap");
		exit(1);
	}

	memcpy(dst, src, statbuf.st_size);  

	close(fdin);
	close(fdout);
	return 0;
}

