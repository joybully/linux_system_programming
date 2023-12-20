#include <stdio.h>
#include <unistd.h>
#include "semlib.h"
 
int semInit(key_t key) {
    /*
    * [Function Name] : int semInit(key_t key)
    * [Input]         :
    *   key_t key
    * [Output]        :
    *   int semid
    * [Call By]       :
    *   None
    * [Calls]         :
    *   None
    * [Given]         :
    *   - System V Semaphore(sys/types.h, sys/ipc.h, sys/sem.h)
    */
    int     semid;

    if ((semid = semget(key, 1, 0600 | IPC_CREAT)) < 0)  {
		perror("semget");
        return -1;
    }

    return semid;
}
 
int semInitValue(int semid, int value) {
    /*
    * [Function Name] : int semInitValue(int semid, int value)
    * [Input]         :
    *   int semid
    *   int value
    * [Output]        :
    * [Call By]       :
    *   None
    * [Calls]         :
    *   None
    * [Given]         :
    *   - System V Semaphore(sys/types.h, sys/ipc.h, sys/sem.h)
    * [Returns]       :
    *   None
    */
    union semun {
            int     val;
    } semun;

    semun.val = value;
    if (semctl(semid, 0, SETVAL, semun) < 0)  { // SETVAL: semaphore
        perror("semctl");
        return -1;
    }
 
    return semid;
}
 
int semWait(int semid) {
    /*
    * [Function Name] : int semWait(int semid)
    * [Input]         :
    *   int semid
    * [Output]        :
    * [Call By]       :
    *   None
    * [Calls]         :
    *   None
    * [Given]         :
    *   - System V Semaphore(sys/types.h, sys/ipc.h, sys/sem.h)
    * [Returns]       :
    *   None
    */
    struct sembuf   semcmd;
 
    semcmd.sem_num = 0;
    semcmd.sem_op = -1;
    semcmd.sem_flg = SEM_UNDO;
    if (semop(semid, &semcmd, 1) < 0)  {
		perror("semop");
        return -1;
    }
 
    return 0;
}
 
int semTryWait(int semid) {
    /*
    * [Function Name] : int semTryWait(int semid)
    * [Input]         :
    *   int semid
    * [Output]        :
    * [Call By]       :
    *   None
    * [Calls]         :
    *   None
    * [Given]         :
    *   - System V Semaphore(sys/types.h, sys/ipc.h, sys/sem.h)
    * [Returns]       :
    *   None
    */
    struct sembuf   semcmd;
 
    semcmd.sem_num = 0;
    semcmd.sem_op = -1;
    semcmd.sem_flg = IPC_NOWAIT | SEM_UNDO; // IPC_NOWAIT: Semaphore
    if (semop(semid, &semcmd, 1) < 0)  {
		perror("semop");
        return -1;
    }
 
    return 0;
}

int semPost(int semid) {
    /*
    * [Function Name] : int semPost(int semid)
    * [Description]   :
    *   - semop system call
    * [Input]         :
    *   int semit
    * [Output]        :
    * [Call By]       :
    *   None
    * [Calls]         :
    *   None
    * [Given]         :
    *   - System V Semaphore(sys/types.h, sys/ipc.h, sys/sem.h)
    * [Returns]       :
    *   None
    */
    struct sembuf   semcmd;
 
    semcmd.sem_num = 0;
    semcmd.sem_op = 1; 
    semcmd.sem_flg = SEM_UNDO;
    if (semop(semid, &semcmd, 1) < 0)  {
		perror("semop");
        return -1;
    }
 
    return 0;
}

int semGetValue(int semid) {
    /*
    * [Function Name] : int semGetValue(int semid)
    * [Description]   :
    *   - semctl system call
    * [Input]         :
    *   int semid
    * [Output]        :
    *   (int) value // Semaphore
    * [Call By]       :
    *   None
    * [Calls]         :
    *   None
    * [Given]         :
    *   - System V Semaphore(sys/types.h, sys/ipc.h, sys/sem.h)
    * [Returns]       :
    *   None
    */
    union semun {
            int     val;
    } dummy;
 
    return semctl(semid, 0, GETVAL, dummy); 
}
 
int semDestroy(int semid) {
    /*
    * [Function Name] : int semDestroy(int semid)
    * [Description]   :
    *   - Semaphore
    *   - semctl system call
    * [Input]         :
    *   int semid
    * [Output]        :
    *   (int result) 
    * [Call By]       :
    *   None
    * [Calls]         :
    *   None
    * [Given]         :
    *   - System V Semaphore(sys/types.h, sys/ipc.h, sys/sem.h)
    * [Returns]       :
    *   None
    */
    union semun {
            int     val;
    } dummy;
 
    if (semctl(semid, 0, IPC_RMID, dummy) < 0)  {  
		perror("semctl");
        return -1;
    }
    close(semid);
 
    return 0;
}
