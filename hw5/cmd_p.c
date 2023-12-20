#include <stdio.h>

#define	MAX_CMD		256

void DoCmd(char *cmd)
{//print cmd execution, wait for 1 second, execute
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done\n");
}

int main()
{
	char	cmd[MAX_CMD];
	int		pid;

	while (1)  {
		printf("CMD> ");//enter the command
		fgets(cmd, MAX_CMD, stdin);	//get the input
		if (cmd[0] == 'q')//it the input starts with q, terminate
			break;

		if ((pid = fork()) < 0)  {//fork
			perror("fork");
			exit(1);
		}
		else if (pid == 0)  {
			DoCmd(cmd);//execute function DoCmd
			exit(0);
		}
#if 0
		else  {
			wait(NULL);
		}
#endif
	}
}
