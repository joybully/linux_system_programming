#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void
CharAtaTime(char *str, int a)
{
	char	*ptr;
	int		c, i;

	setbuf(stdout, NULL);
	for (ptr = str ; c = *ptr++ ; )  {
		for(i = 0 ; i < 999999 ; i++)
			;
		putc(c, stdout);
	}
	putc((char)a,stdout);
}

int main(){
	
	pid_t pid;
	
	for(int i=0;i<4; i++){
		if((pid=fork())<0){
			perror("fork");
		}
		else if(pid==0){
			CharAtaTime("output from child\n",i+1);
		}
		else{
			wait(NULL);
				CharAtaTime("output from parent\n",i);
		}
	}
	return 0;
}
