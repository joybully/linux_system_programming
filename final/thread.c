#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

BoundedBufferType	Buf;
sem_t				EmptySem, FullSem, MutexSem;
int MAX_MSG = 80;
char Msg[80];

void ThreadUsleep(int usecs) {

	/*
	* [Function Name]: void ThreadUsleep(int usecs)
	* [Description]	 :
	*	- μs단위로 thread를 슬립시킴
	* [Input]		 :
	*	int usecs
	* [Output]		 :
	*	None
	* [Call By]		 :
	*	void Producer(void* dummy)
	*	void Consumer(void* dummy)
	* [Calls]		 :
	*	None
	* [Given]		 :
	*	None
	* [Returns]		 :
	*	None
	*/
	pthread_cond_t		cond;
	pthread_mutex_t		mutex;
	struct timespec		ts;
	struct timeval		tv;

	if (pthread_cond_init(&cond, NULL) < 0)  {
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if (pthread_mutex_init(&mutex, NULL) < 0)  {
		perror("pthread_mutex_init");
		pthread_exit(NULL);
	}

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + usecs/1000000;
	ts.tv_nsec = (tv.tv_usec + (usecs%1000000)) * 1000;
	if (ts.tv_nsec >= 1000000000)  {
		ts.tv_nsec -= 1000000000;
		ts.tv_sec++;
	}

	if (pthread_mutex_lock(&mutex) < 0)  {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	if (pthread_cond_timedwait(&cond, &mutex, &ts) < 0)  {
		perror("pthread_cond_timedwait");
		pthread_exit(NULL);
	}

	if (pthread_cond_destroy(&cond) < 0)  {
		perror("pthread_cond_destroy");
		pthread_exit(NULL);
	}
	if (pthread_mutex_destroy(&mutex) < 0)  {
		perror("pthread_mutex_destroy");
		pthread_exit(NULL);
	}
}

void Producer(void *dummy) {
	/*
	* [Function Name]: void Producer(void* dummy)
	* [Description]	 :
	*	- 버퍼 내에 랜덤값을 NLOOPS개 집어넣음
	*	- 세마포어
	* [Input]		 :
	*	None
	* [Output]		 :
	*	int data	// 큐에 들어갈 랜덤 데이터
	* [Call By]		 :
	*	int main()
	* [Calls]		 :
	*	void ThreadUsleep(int usecs)
	* [Given]		 :
	*	(int) NLOOPS
	*	BoundedBufferType Buf
	* [Returns]		 :
	*	Buf.buf[n]	// 큐(버퍼)내에 들어갈 데이터
	*/
	int		i, data;


	while(Msg[0]!='q'||Msg[1]!='u'||Msg[2]!='i'||Msg[3]!='t'){
		if (sem_wait(&EmptySem) < 0)  {
			perror("sem_wait");
			pthread_exit(NULL);
		} // busy waiting 을 없앰 
		if (sem_wait(&MutexSem) < 0)  {
			perror("sem_wait");
			pthread_exit(NULL);
		}
		while(1){
			printf("In> ");
			fgets(Msg,MAX_MSG, stdin); 
			//if(Msg[0]=='q'&&Msg[1]=='u'&&Msg[2]=='i'&&Msg[3]=='t')
				break;
		}

		if (sem_post(&MutexSem) < 0)  { // 나가는 세마포어 (post)
			perror("sem_post");
			pthread_exit(NULL);
		}
		if (sem_post(&FullSem) < 0)  { // 채워놨으니 full 을 하나 증가시킴 
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep(data);
	}
	
	pthread_exit(NULL);
}

void Consumer(void *dummy) {
	/*
	* [Function Name]: void Consumer(void* dummy)
	* [Description]	 :
	*	- 버퍼 내에 랜덤값을 NLOOPS개 가져옴
	*	- 세마포어 이용
	* [Input]		 :
	*	None
	* [Output]		 :
	*	int data	// 큐에서 가져올 데이터
	* [Call By]		 :
	*	int main()
	* [Calls]		 :
	*	void ThreadUsleep(int usecs)
	* [Given]		 :
	*	(int) NLOOPS
	*	BoundedBufferType Buf
	* [Returns]		 :
	*	None
	*/
	int		i, data;

	printf("Consumer: Start.....\n");

	char pr[6];
	while(Msg[0]!='q'||Msg[1]!='u'||Msg[2]!='i'||Msg[3]!='t'){
		if (sem_wait(&FullSem) < 0)  {
			perror("sem_wait");
			pthread_exit(NULL);
		}
		if (sem_wait(&MutexSem) < 0)  {
			perror("sem_wait");
			pthread_exit(NULL);
		}

		char a;
		int k = 0;
		printf("Out> ");
		while((a=Msg[k])!='\0'){
			if(a>='a'&&a<='z'){
				a-=32;
			}
			printf("%c",a);
			k++;
		}
		printf("\n");

		if (sem_post(&MutexSem) < 0)  {
			perror("sem_post");
			pthread_exit(NULL);
		}
		if (sem_post(&EmptySem) < 0)  {
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep((rand()%100)*10000);
	}

	printf("Consumer: %s", pr);

	pthread_exit(NULL);
}

int main() {
	/*
	* [Description]	:
	*	- Thread 버전의 producer-consumer Task
	* [Input]		:
	*	None
	* [Output]		:
	*	(int) pBuf->counter	// 버퍼에 남은 데이터의 개수
	* [Calls]		:
	*	void Producer(void *dummy)
	*	void Consumer(void *dummy)
	* [특기사항]	:
	*	None
	*/
	pthread_t	tid1, tid2;

	srand(0x9999);

	if (sem_init(&EmptySem, 0, MAX_BUF) < 0)  {
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&FullSem, 0, 0) < 0)  {
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&MutexSem, 0, 1) < 0)  {
		perror("sem_init");
		exit(1);
	} // 각 세마포어 3개 생성

	if (pthread_create(&tid1, NULL, (void *)Producer, (void *)NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, (void *)Consumer, (void *)NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_join(tid1, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}

	if (sem_destroy(&EmptySem) < 0)  {
		perror("sem_destroy");
	}
	if (sem_destroy(&FullSem) < 0)  {
		perror("sem_destroy");
	}
	if (sem_destroy(&MutexSem) < 0)  {
		perror("sem_destroy");
	}

	return 0;
}

