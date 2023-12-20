#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int		Var = 6;
char	Buf[] = "a write to stdout\n";

int main()
{
	int		var;
	pid_t	pid;

	var = 88;
	write(STDOUT_FILENO, Buf, sizeof(Buf)-1);//write the file
	printf("Before fork\n");

	if ((pid = fork()) < 0)  {//fork process
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  {//increase Var twice
		/* child */
		Var++;
		var++;
	}
	else  {
		/* parent *///wait for 2 seconds(for the child to be terminated)
		sleep(2);
	}

	printf("pid = %d, Var = %d, var = %d\n", getpid(), Var, var);
}
