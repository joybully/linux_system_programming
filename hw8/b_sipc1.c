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
	printf("Received request: %s.....", pData);
	sprintf(pData, "This is a reply from %d.", getpid());
	printf("Replied.\n");


	if(semPost(mutexSemid)<0){
		fprintf(stderr,"semPost failure\n");
		exit(1);
	}
	if(semPost(fullSemid)<0){
		fprintf(stderr,"semPost failure\n");
		exit(1);
	}
	sleep(1);	
	if(semDestroy(emptySemid)<0){
		fprintf(stderr, "semDestroy failure\n");
	}
	if(semDestroy(fullSemid)<0){
		fprintf(stderr, "semDestroy failure\n");
	}
	if(semDestroy(mutexSemid)<0){
		fprintf(stderr, "semDestroy failure\n");
	}

	if (shmctl(shmid, IPC_RMID, 0) < 0)  {
		perror("shmctl");
		exit(1);
	}
    return 0;
}
