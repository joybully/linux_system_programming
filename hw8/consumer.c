#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"

int main() { 
	/*
	* [Description]	:
	*	- Process 버전의 producer-consumer Task
	*	- Consumer Process 생성, 버퍼에 있는 값을 꺼냄
	* [Input]		:
	*	None
	* [Output]		:
	*	(int) pBuf->counter	// 버퍼에 남은 데이터의 개수
	* [Calls]		:
	*	None
	* [특기사항]	:
	*	- 버퍼가 비어있을 경우 대기(Busy Waiting)
	*	- 버퍼(BoundedBufferType)와 루프 횟수(NLOOPS)가 정의되어 있어야함
	*	- Synchronization X -> Critical Section Problem이 발생할 수 있음
	*/
	BoundedBufferType	*pBuf;
	int					shmid, i, data;

	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}
	if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1)  {
		perror("shmat");
		exit(1);
	} // 여기까지는 producer 와 같음 

	srand(0x9999);
	for (i = 0 ; i < NLOOPS ; i++)  {
		if (pBuf->counter == 0)  {
			printf("Consumer: Buffer empty. Waiting.....\n");
			while (pBuf->counter == 0);
		}

		printf("Consumer: Consuming an item.....\n");
		data = pBuf->buf[pBuf->out].data;
		pBuf->out = (pBuf->out + 1) % MAX_BUF;
		pBuf->counter--;

		usleep((rand() % 100) * 10000);
	}

	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", pBuf->counter);
	return 0;
}

