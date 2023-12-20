#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int mysystem(char* cmd) {//mysystem to execute file
	int             status;
	char* argv[]  = {"ls","-al","/tmp",NULL}; //command string for execution
	execv(cmd,argv); //argv gonna be command line
}

int main(){
	pid_t pid;
	int status;
	char* cmd = "/bin/ls"; //this is cmd
	
	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}

	else {
		waitpid(pid,&status,0); //wait until child process terminate
		mysystem(cmd);
	}

	return 0;
}

