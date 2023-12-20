#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	pid_t	pid;
	int		status;

	//fork
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	
	//wait for 2seconds
	else if (pid == 0)  {
		printf("I'm a child\n");
		sleep(2);
	}

	//wait for child process to be terminated
	else  {
		wait(&status);
		printf("A child killed with %d status\n", status);
	}
}
