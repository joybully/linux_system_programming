#include <stdio.h>
#include <pthread.h>


void PrintMsg(char *msg)//print msg, thread termination
{
	printf("%s", msg);

	pthread_exit(NULL);
}

int main()
{
	pthread_t	tid1, tid2;//thread pid
	char		*msg1 = "Hello, ";
	char		*msg2 = "World!\n";

	//create thread and print with function PrintMsg
	if (pthread_create(&tid1, NULL, (void *)PrintMsg, (void *)msg1) < 0)  {
		perror("pthread_create");
		exit(1);
	}

	//create thread and print with function PrintMsg
	if (pthread_create(&tid2, NULL, (void *)PrintMsg, (void *)msg2) < 0)  {
		perror("pthread_create");
		exit(1);
	}
	
	//print the threads created
	//executed one by one, if it was not thread, strings gonna be mixed
	printf("Threads created: tid=%d, %d\n", tid1, tid2);
	
	//join the thread
	if (pthread_join(tid1, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}

	//join the thread
	if (pthread_join(tid2, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}

	printf("Threads terminated: tid=%d, %d\n", tid1, tid2);
}
