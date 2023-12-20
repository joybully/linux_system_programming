#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "synclib.h"

#define	MAX_BUF	128

int main() {
	int		n, fd[2];
	pid_t	pid;
	char	buf[MAX_BUF];
	int		n2, fd2[2];
	char	buf2[MAX_BUF];

	if (pipe(fd) < 0)  {
		perror("pipe");
		exit(1);
	}
	
	if(pipe(fd2)<0){
		perror("pipe");
		exit(1);
	}

	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  {
		close(fd[1]);
		close(fd2[0]);
		strcpy(buf2, "From Child : Hello, Parent!\n");
		printf("Child : Send data to parent\n");
		write(fd2[1], buf2, strlen(buf2)+1);
		printf("Child : Wait for parent to send data\n");
		if ((n = read(fd[0], buf, MAX_BUF)) < 0)  {
			perror("read");
			exit(1);
		}
		printf("Child : Received data from parent\n");
		fflush(stdout);
		write(STDOUT_FILENO, buf, n);
	}
	else  {
		close(fd[0]);
		close(fd2[1]);
		strcpy(buf, "From Parent : Hello, Child!\n");
		printf("Parent : Send data to child\n");
		write(fd[1], buf, strlen(buf)+1);
		printf("Parent : Wait for Child to send data\n");
		if((n2 = read(fd2[0], buf2, MAX_BUF))<0){
			perror("read");
			exit(1);
		}
		printf("Parent : Received data from Child\n");
		fflush(stdout);
		write(STDOUT_FILENO, buf2, n2);
	}

	exit(0);
	return 0;
}
