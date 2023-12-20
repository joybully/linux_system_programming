#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

static unsigned int	AlarmSecs;//set time


void SigAlarmHandler(int signo)
{
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR)  {
		perror("signal");
		exit(1);
	}//works in only solaris, not linux

	alarm(AlarmSecs);

	printf(".");//one dot in one second
	fflush(stdout);
	return;
}

int SetPeriodicAlarm(unsigned int nsecs)
{
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR)  {
		return -1;
	}

	AlarmSecs = nsecs;

	alarm(nsecs);

	return 0;
}

int main()
{
	printf("Doing something every one seconds\n");

	SetPeriodicAlarm(1);//act every second

	for ( ; ; )
		pause();

	return 0;
}
