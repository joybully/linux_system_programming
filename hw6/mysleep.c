#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void SigAlarmHandler(int signo) {
	//do nothing, wake up the pause
	return;
}

unsigned int mysleep(unsigned int nsecs) {
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR)  {
		return nsecs;
	}

	alarm(nsecs);//wake up after 5 seconds
	pause(); //use pause

	return alarm(0);//send signal after 0 times
}

int main() {
	printf("Wait for 5 seconds...\n");
	mysleep(5);//wait for 5 seconds

	return 0;
}
