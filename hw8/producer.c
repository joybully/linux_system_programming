#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"

int main() {
	/*
	* [Program Name]: Assignment 9-2
	* [Description]	:
	*	- Process 버전의 producer-consumer Task
	*	- Producer Process 생성, 랜덤값을 버퍼에 NLOOPS번 넣음
	* [Input]		:
	*	None
	* [Output]		:
	*	(int) pBuf->counter	// 버퍼에 남은 데이터의 개수
	* [Calls]		:
	*	None
	* [특기사항]	:
	*	- 버퍼가 가득 차있을 경우 대기(Busy Waiting)
	*	- 버퍼(BoundedBufferType)와 루프 횟수(NLOOPS)가 정의되어 있어야함
	*	- Synchronization X -> Critical Section Problem이 발생할 수 있음
	*/
	BoundedBufferType	*pBuf; // 구조체 크기 만큼 shared memory 만들어줌 
	int					shmid, i, data;

	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  { // shared memory 설정
		perror("shmget");
		exit(1);
	}
	if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1)  { // 포인터가 바로 shared memory 공간 대신 쓰는 공간은 BoundedBufferType 만큼
		perror("shmat");
		exit(1);
	}
	srand(0x8888);
	for (i = 0 ; i < NLOOPS ; i++)  {	// 랜덤 data를 생성하여 버퍼에 넣는 task를 NLOOPS번 반복
		if (pBuf->counter == MAX_BUF)  {
			printf("Producer: Buffer full. Waiting.....\n");
			while (pBuf->counter == MAX_BUF);
		}

		printf("Producer: Producing an item.....\n");
		data = (rand() % 100) * 10000;
		pBuf->buf[pBuf->in].data = data;
		pBuf->in = (pBuf->in + 1) % MAX_BUF;
		pBuf->counter++;

		usleep(data); // process 여서 그냥 usleep 사용함 ( 쓰레드 말고 )
	}

	printf("Producer: Produced %d items.....\n", i);

	sleep(2); // 컨슈머가 종료되는 시간 
	printf("Producer: %d items in buffer.....\n", pBuf->counter);
	
	if (shmctl(shmid, IPC_RMID, 0) < 0)  {
		perror("shmctl");
		exit(1);
	}
	return 0;
}

