#include <pthread.h>

typedef struct  {
	int		sval;
	pthread_mutex_t	mutex; // critical section 을 보호할 mytex
	pthread_cond_t	cond; // 기다리기 위한 condition 변수
}
	sem_t;
// 이전에 사용했던 posix 세마포오 부분 
int		sem_init(sem_t *sem, int pshared, int value);
int		sem_wait(sem_t *sem);
int		sem_trywait(sem_t *sem);
int		sem_post(sem_t *sem);
int		sem_getvalue(sem_t *sem, int *sval);
int		sem_destroy(sem_t *sem);
