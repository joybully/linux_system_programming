#include <stdio.h>
#include <pthread.h>
#include "semlib2.h"

int sem_init(sem_t *sem, int pshared, int value) { // pshared 가 0이면 지원 안함 
	if (pshared)  {
		fprintf(stderr, "Function not supported\n");
		return -1;
	}

	if (pthread_mutex_init(&sem->mutex, NULL) < 0) // mutex 생성
		return -1;

	if (pthread_cond_init(&sem->cond, NULL) < 0) // cond 생성
		return -1;

	sem->sval = value;

	return 0;
}

int sem_wait(sem_t *sem) { // 메인이 값을 1 시키는 것 
	if (pthread_mutex_lock(&sem->mutex) < 0) // 들어갈때 lock 검
		return -1;

	if (sem->sval == 0)  {
		while (sem->sval == 0)  { // 여러 프로듀서 일 수 있기 때문에 while 
			if (pthread_cond_wait(&sem->cond, &sem->mutex) < 0) {// wait 가 0이면 기다리고 0이 아니면 1을 뺌 (cond 기다림)
				if (pthread_mutex_unlock(&sem->mutex) < 0) // lock 을 잠시 풀어줌 
					return -1;
				return -1;
			}
		}
		sem->sval--;
	}
	else  {
		sem->sval--;
	}

	if (pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;

	return 0;
}

int sem_trywait(sem_t *sem) {
	if (pthread_mutex_lock(&sem->mutex) < 0)
		return -1;

	if (sem->sval == 0)  {
		if (pthread_mutex_unlock(&sem->mutex) < 0)
			return -1;
		return -1;
	}
	else  {
		sem->sval--;
	}

	if (pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;

	return 0;
}

int sem_post(sem_t *sem) {
	if (pthread_mutex_lock(&sem->mutex) < 0) //
		return -1;

	if (sem->sval == 0)  { // 누군가 기다리고 있다는 뜻 
		if (pthread_cond_signal(&sem->cond) < 0)  { // 기다리고 있으면 cond 변환
			if (pthread_mutex_unlock(&sem->mutex) < 0)
				return -1;
			return -1;
		}
	}

	sem->sval++;

	if (pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;

	return 0;
}

int sem_getvalue(sem_t *sem, int *sval) { // 성공하면 0을 리턴이고 실패하면 -1 리턴 
	*sval = sem->sval;
	return 0;
}

int sem_destroy(sem_t *sem) {
	if (pthread_mutex_destroy(&sem->mutex) < 0)
		return -1;

	if (pthread_cond_destroy(&sem->cond) < 0)
		return -1;

	return 0;
}


