#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>


void SigChldHandler(int signo){//sigchild handler
	printf("Received a SIGCHLD signal\n");
        printf("Terminate this process\n");
	printf("A child killed\n");
	exit(0);
}

int main() {
	pid_t	pid;

	signal(SIGCHLD,SigChldHandler);//send signal to sigchild
	
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid == 0){		
		printf("I'm a child\n");
		printf("2 seconds.....\n");
		sleep(2);
	}
	else  {
		while(1)//infinite loop
			pause();
	}
	return 0;
}
