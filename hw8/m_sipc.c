#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "semlib2.h"
#include "prodcons.h"

BoundedBufferType	Buf;
sem_t				EmptySem, FullSem, MutexSem;
char Data[256];

void ThreadUsleep(int usecs) {
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

void Parent(void *dummy)
{
	if (sem_wait(&EmptySem) < 0)  {
		perror("sem_wait");
		pthread_exit(NULL);
	}
	if (sem_wait(&MutexSem) < 0)  {
		perror("sem_wait");
		pthread_exit(NULL);
	}
	printf("Received request: %s...",Data);
	sprintf(Data,"This is a request from %d.", getpid());
	printf("Replied.\n");

	if(sem_post(&MutexSem)<0){
		perror("sem_post");
		pthread_exit(NULL);
	}
	if(sem_post(&FullSem)<0){
		perror("sem_post");
		pthread_exit(NULL);
	}		
	
	ThreadUsleep(2);
	pthread_exit(NULL);
}

void Child(void *dummy) {
	if (sem_wait(&FullSem) < 0)  {
		perror("sem_wait");
		pthread_exit(NULL);
	}
	if (sem_wait(&MutexSem) < 0)  {
		perror("sem_wait");
		pthread_exit(NULL);
	}
	
	sprintf(Data,"This is a request from %d.",getpid());
	printf("Sent a request...");
	if (sem_post(&MutexSem) < 0)  {
		perror("sem_post");
		pthread_exit(NULL);
	}
	if (sem_post(&EmptySem) < 0)  {
		perror("sem_post");
		pthread_exit(NULL);
	}
	
	if(sem_wait(&FullSem)<0){
		perror("sem_post");
		pthread_exit(NULL);
	}
	if(sem_wait(&MutexSem)<0){
		perror("sem_post");
		pthread_exit(NULL);
	}
	printf("Received reply: %s\n",Data);

	if(sem_post(&MutexSem)<0){
		perror("sem_post");
		pthread_exit(NULL);
	}
	ThreadUsleep(2);
	pthread_exit(NULL);
}

int main() {
	pthread_t	tid1, tid2;

	srand(0x8888);

	if (sem_init(&EmptySem, 0, 0) < 0)  {
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&FullSem, 0, 1) < 0)  {
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&MutexSem, 0, 1) < 0)  {
		perror("sem_init");
		exit(1);
	}

	if (pthread_create(&tid1, NULL, (void *)Child, (void *)NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, (void *)Parent, (void *)NULL) < 0)  {
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

