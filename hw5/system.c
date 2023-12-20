#include <stdio.h>
#include <stdlib.h>

int main(){
	int		status;

	if ((status = system("date")) < 0)  {//call the system call "date"
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);//call status on date

	if ((status = system("nosuchcommand")) < 0)  {//call the system call "nosuchcommand"
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);

	if ((status = system("who; exit 44")) < 0)  {//call the system call "who; exit 44"
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);
}
