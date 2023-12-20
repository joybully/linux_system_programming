#include <stdio.h>
#include <pthread.h>

#define	MAX_CMD		256

void DoCmd(char *cmd)
{
	printf("Doing %s", cmd);//print input command
	sleep(1);
	printf("Done\n");

	pthread_exit(NULL);
}

int main()
{
	char		cmd[MAX_CMD];
	pthread_t	tid;

	while (1)  {
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin);	
		if (cmd[0] == 'q')
			break;

		if (pthread_create(&tid, NULL, (void *)DoCmd, (void *)cmd) < 0)  {
		//create thread and execute command input DoCmd, cmd is fucntion name and fuction parameter
			perror("pthread_create");
			exit(1);
		}

#if 0
		pthread_join(tid, NULL);
#endif
	}
}
