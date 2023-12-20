#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void SigIntHandler(int signo) {//print signumber according to signal handler
	printf("Received a SIGINT signal\n");
	printf("Terminate this process\n");
	exit(0);
}

int main() {
	signal(SIGINT, SigIntHandler);//signal state

	printf("Press ^C to quit\n");

	for ( ; ; )//infinite loop(what signal do)
		pause();
	return 0;
}

