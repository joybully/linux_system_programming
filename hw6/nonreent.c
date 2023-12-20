#include <stdio.h>
#include <signal.h>
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void MyAlarmHandler(int signo) {//similar with alarm.c
	struct passwd	*rootptr;

	signal(SIGALRM, MyAlarmHandler);
	alarm(1);

	printf("in signal handler\n");

	if ((rootptr = getpwnam("root")) == NULL)  { 
		perror("getpwnam");
		exit(1);
	}

	return;
}

int main() {
	struct passwd	*ptr;

	signal(SIGALRM, MyAlarmHandler);//get the signal
	alarm(1);

	for ( ; ; ) {//infinite loop
		if ((ptr = getpwnam("lsp14")) == NULL)  {
			//non reentrant occur
			//root will using shared space after lsp
			perror("getpwnam");
			exit(1);
		}

		if (strcmp(ptr->pw_name, "lsp14") != 0)  {
			printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
			exit(0);
		}
	}
	return 0;
}

