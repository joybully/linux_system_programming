#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void SigUsrHandler(int signo) {
	if (signo == SIGUSR1)  { //case of signal number is SIGUSER1
		printf("Received a SIGUSR1 signal\n");
	}
	else if (signo == SIGUSR2)  {//case of signal number is SIGUSER2
		printf("Received a SIGUSR2 signal\n");
	}
	else  {
		printf("Received unknown signal\n");
		printf("Terminate this process\n");
		exit(0);
	}
}

int main() {
	if (signal(SIGUSR1, SigUsrHandler) == SIG_ERR)  {//show error if there's no SIGUSR1
		perror("signal");
		exit(1);
	}

	if (signal(SIGUSR2, SigUsrHandler) == SIG_ERR)  {//show error if there's no SIGUSR2
		perror("signal");
		exit(1);
	}

	for ( ; ; )
		pause();

	return 0;
}

