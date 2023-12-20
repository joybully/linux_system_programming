#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	int		pid;

	if ((pid = fork()) < 0)  {//fork
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  {//if the pid is 0, call the child process
		/* child */
		printf("I am %d. My parent is %d.\n", getpid(), getppid());
	}
	else  {//if the pid is 1, call the parent process
		/* parent */
		printf("I am %d. My child is %d.\n", getpid(), pid);
	}
	return 0;
}
