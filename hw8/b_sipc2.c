#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"
#include "semlib.h"

int main() {
	BoundedBufferType *pBuf;
	int		shmid;
	char	*pData;
	int     emptySemid, fullSemid, mutexSemid;
  
    if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
          perror("shmget");
          exit(1);
    }
    if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1)  {
          perror("shmat");
          exit(1);
    }
  
    if ((emptySemid = semInit(EMPTY_SEM_KEY)) < 0)  {
          fprintf(stderr, "semInit failure\n");
          exit(1);
    }
    if ((fullSemid = semInit(FULL_SEM_KEY)) < 0)  {
          fprintf(stderr, "semInit failure\n");
          exit(1);
    }
    if ((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0)  {
          fprintf(stderr, "semInit failure\n");
          exit(1);
    }
  
    if (semInitValue(emptySemid, MAX_BUF) < 0)  {
          fprintf(stderr, "semInitValue failure\n");
          exit(1);
    }
    if (semInitValue(fullSemid, 0) < 0)  {
          fprintf(stderr, "semInitValue failure\n");
          exit(1);
    }
    if (semInitValue(mutexSemid, 1) < 0)  {
          fprintf(stderr, "semInitValue failure\n");
          exit(1);
     }


	pData = pBuf + sizeof(int);
	sprintf(pData, "This is a request from %d.", getpid());
	printf("Sent a request....\n");


	if(semPost(mutexSemid)<0){
		fprintf(stderr,"semPost failure\n");
		exit(1);
	}
	if(semPost(emptySemid)<0){
		fprintf(stderr,"semPost failure\n");
		exit(1);
	}
	if (semWait(fullSemid) < 0)  {
          fprintf(stderr, "semWait failure\n");
          exit(1);
	}
	if (semWait(mutexSemid) <0) {
          fprintf(stderr, "semInit failure\n");
          exit(1);
    }
  
     printf("Received reply: %s\n", pData);
     return 0;
}
