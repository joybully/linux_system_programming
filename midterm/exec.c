#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
int main(){
	pid_t pid;
	char *argv[] = {"print","HelloWorld!", "6",NULL};
	//execl
	if((pid=fork())<0){
		perror("fork");
		exit(1);
	}
	else if(pid==0){
		if(execl("/home/lsp14/midterm/print","print","HelloWorld!","6",NULL)<0){
			perror("execl");
			exit(1);
		}
	}
	if(waitpid(pid,NULL,0)<0){
		perror("waitpid");
		exit(1);
	}

	//exev
	if((pid=fork())<0){
		perror("fork");
		exit(1);
	}
	else if(pid==0){
		if(execv("/home/lsp14/midterm/print", argv)<0){
			perror("execv");
			exit(1);
		}
	}
	if(waitpid(pid,NULL,0)<0){
		perror("waitpid");
		exit(1);
	}
	return 0;
}
