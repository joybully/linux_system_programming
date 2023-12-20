#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

BoundedBufferType	Buf;

void ThreadUsleep(int usecs) { // 동시제어 부분 
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
	*	- 버퍼가 가득차있을 경우 자리가 날 때까지 대기(Busy waiting)
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
	int	i, data;

	printf("Producer: Start.....\n"); // 시작 출력

	for (i = 0 ; i < NLOOPS ; i++)  { // NLOOPS 만큼 반복
		if (Buf.counter == MAX_BUF) { // 가득 차있으면 대기
			printf("fProducer : Buffer full, Waiting. . . \n"); // 버퍼가 기다리고 있는지 눈으로 보이게끔 해주는 프린트 
			while (Buf.counter == MAX_BUF);
		}

		printf("Producer: Producing an item.....\n");
		data = (rand() % 100) * 10000; // 랜덤 데이터 (0~99)*10000 -> 0~1초 사이에 msec 가 나옴 
		Buf.buf[Buf.in].data = data; // 구조체 
		Buf.in = (Buf.in + 1) % MAX_BUF;
		Buf.counter++;

		ThreadUsleep(data); // 쉬지않고 계속 집어넣으면 안되기 때문에 랜덤한 시간동안 기다리는 함수 (쓰레드 단위로 msec동안 sleep)
	}

	printf("Producer: Produced %d items.....\n", i);
	printf("Producer: %d items in buffer.....\n", Buf.counter);
	
	pthread_exit(NULL);
}

void Consumer(void *dummy) {
	/*
	* [Function Name]: void Consumer(void* dummy)
	* [Description]	 :
	*	- 버퍼 내에 랜덤값을 NLOOPS개 가져옴
	*	- 버퍼가 비어있을 경우 데이터가 생길 때까지 대기(Busy waiting)
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

	for (i = 0 ; i < NLOOPS ; i++)  {
		while (Buf.counter == 0)
			;

		printf("Consumer: Consuming an item.....\n");
		data = Buf.buf[Buf.out].data;
		Buf.out = (Buf.out + 1) % MAX_BUF;
		Buf.counter--; // buf에서 뺌(데이터를)

		ThreadUsleep((rand()%100)*10000);
	}

	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL);
}

// 큐 사이즈는 2로 되어있음 
int main() {
	/*
	* [Program Name]: Assignment 9-1
	* [Description]	:
	*	- Thread 버전의 producer-consumer Task
	*	- Producer Thread, Consumer Therad를 만들고 두 스레드가 종료될 때까지 대기 후 종료
	* [Input]		:
	*	None
	* [Output]		:
	*	(int) Buf.counter	// 버퍼에 남은 데이터의 개수
	* [Calls]		:
	*	void Producer
	*	void Consumer
	* [특기사항]	:
	*	- Critical Section Problem이 발생할 수 있음
	*/
	pthread_t	tid1, tid2;

	srand(0x8888); // 랜덤 변수 

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

	printf("Main    : %d items in buffer.....\n", Buf.counter); // 마지막 종료될때 buf에 몇개가 남아있는지 출력 
	return 0;
}

