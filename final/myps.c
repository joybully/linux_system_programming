#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>

int  main(int argc, char *argv[] )  {
	int fd[2];
	pid_t child;

	if(pipe(fd) == -1)  {
		perror("pipe");
		exit(1);
	}
 
	child = fork();
	if(child == (pid_t)(-1))  {
        	perror("fork");
		exit(1);
	}
	else if(child == (pid_t)0) {
        	close(1);
			close(fd[0]);
        	if(dup(fd[1]) == -1)  {
        		perror("dup");
				exit(1);
			}

        	if((execlp("ps", "ps", "-ef", NULL)) == -1)  {
        		perror("execlp");
				exit(1);
			}
	} 
	else {
        	close(0);
			close(fd[1]);
        	
        	if(dup(fd[0]) == -1)  {
        		perror("dup");
				exit(1);
			}
        	if((execlp("grep", "grep",argv[1], NULL)) == -1)  {
        		perror("execlp");
				exit(1);
			}
	}
 
	return 0;
}

