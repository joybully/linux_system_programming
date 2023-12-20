#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

void myusleep(int secs, int usecs) {
	/*
	* [Function Name]: void myusleep(int secs, int usecs)
	* [Description]	 :
	*	- tm��ŭ sleep�ߴٰ� �Ͼ
	* [Input]		 :
	*	int secs
	*	int usecs
	* [Output]		 :
	*	None
	* [Call By]		 :
	*	int main()
	* [Calls]		 :
	*	None
	* [Given]		 :
	*	None
	* [Returns]		 :
	*	None
	*/
	struct timeval	tm;

	tm.tv_sec = secs;
	tm.tv_usec = usecs;

	if (select(0, (fd_set *)0, (fd_set *)0, (fd_set *)0, &tm) < 0) {
		perror("select");
		return;
	}
}

int main() {
	/*
	
	* [Description]	:
	*	
	* [Input]		:
	*	None
	* [Output]		:
	*	None
	* [Calls]		:
	*	void myusleep(int secs, int usecs)
	* 
	*
	*/
	printf("Sleep for 1.5 seconds.....\n");
	myusleep(1, 500000);
	return 0;
}

