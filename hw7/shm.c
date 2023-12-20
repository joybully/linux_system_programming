#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

#define	ARRAY_SIZE	40000
#define	MALLOC_SIZE	100000
#define	SHM_SIZE	100000
#define	SHM_MODE	(SHM_R | SHM_W)

char	Array[ARRAY_SIZE];

int main() {
	int		shmid;
	char	*ptr, *shmptr;

	if ((ptr = (char *)malloc(MALLOC_SIZE)) == NULL)  { //this points heap
		perror("malloc");
		exit(1);
	}

	if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0)  { //first parameter will be used only in this code, if this process killed, memory will be initilized automatically
		perror("shmget");
		exit(1);
	}
	if ((shmptr = shmat(shmid, 0, 0)) == (void *) -1)  { //assign address
		perror("shmat");
		exit(1);
	}

	printf("Array[] from %s to %s\n", &Array[0], &Array[ARRAY_SIZE]); //print addrss of array global variable 
	printf("Stack around %ls\n", &shmid);
	printf("Malloced from %s to %s\n", ptr, ptr+MALLOC_SIZE);
	printf("Shared memory attached from %s to %s\n", shmptr, shmptr+SHM_SIZE);

	if (shmctl(shmid, IPC_RMID, 0) < 0)  { //remove shm(if IPC_PRIVATE is used, this code is useless)
		perror("shmctl");
		exit(1);
	}

	return 0;
}

