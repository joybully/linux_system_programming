#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>

void SigChldHandler(int signo){//sigchild handler
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
		sleep(1);
		int count = 0;
		while(1){
			count++;
		}
		printf("%d\n", count);
	}
	else  {
		while(1)
			pause();
	}
	return 0;
}
