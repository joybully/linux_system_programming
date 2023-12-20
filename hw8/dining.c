#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

#define	NUM_MEN		5
#define	NLOOPS		5

sem_t	ChopStick[NUM_MEN];

void ThreadUsleep(int usecs) { // 중간중간 텀을 주기 위한 함수 (usec) 쓰레드별로 동작을 기다리도록 
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

	gettimeofday(&tv, NULL); // 현재 시각
	ts.tv_sec = tv.tv_sec + usecs/1000000; // 현재 시각 + 기다리는 시간 
	ts.tv_nsec = (tv.tv_usec + (usecs%1000000)) * 1000;
	if (ts.tv_nsec >= 1000000000)  {
		ts.tv_nsec -= 1000000000;
		ts.tv_sec++;
	}

	if (pthread_mutex_lock(&mutex) < 0)  {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	if (pthread_cond_timedwait(&cond, &mutex, &ts) < 0)  { // 지정된 시간 만큼 cond 를 기다리다가 그 시간이 증가하면 그냥 빠져나옴/ 이것을 사용하려면 cond 와 mutex가 필요함 (절대 시간 이용, ts) 
		perror("pthread_cond_timedwait");
		pthread_exit(NULL);
	}

	if (pthread_cond_destroy(&cond) < 0)  { // 더이상 사용 안하기 때문에 destroy시킴
		perror("pthread_cond_destroy");
		pthread_exit(NULL);
	}
	if (pthread_mutex_destroy(&mutex) < 0)  {
		perror("pthread_mutex_destroy");
		pthread_exit(NULL);
	}
}

void Thinking(int id) {
	printf("Philosopher%d: Thinking.....\n", id);
	ThreadUsleep((rand()%200)*10000); // 생각하는 시간 (랜덤시간)
	printf("Philosopher%d: Want to eat.....\n", id); // 먹기를원함
}

void Eating(int id) {
	printf("Philosopher%d: Eating.....\n", id);
	ThreadUsleep((rand()%100)*10000);
}

void DiningPhilosopher(int *pId) { // 세마포오 아이디  이용 쓰레드에 아이디는 무조건 포인터 이용
	int		i;
	int		id = *pId;

	for (i = 0 ; i < NLOOPS ; i++)  {

		Thinking(id);

		if (sem_wait(&ChopStick[id]) < 0)  { // wait(왼쪽 젓가락)  왼쪽에 없으면 실패
			perror("sem_wait");
			pthread_exit(NULL);
		}
		if (sem_wait(&ChopStick[(id+1) % NUM_MEN]) < 0)  { // 오른쪽젓가락 오른쪽 젓가락이 없으면 실패 
			perror("sem_wait");
			pthread_exit(NULL);
		}

		Eating(id);  // 둘다 있을경우 eating 

		if (sem_post(&ChopStick[id]) < 0)  { // 사용 후 내려놓음 왼쪽
			perror("sem_post");
			pthread_exit(NULL);
		}
		if (sem_post(&ChopStick[(id+1) % NUM_MEN]) < 0)  {
			perror("sem_post");
			pthread_exit(NULL);
		}
	}

	printf("Philosopher%d: thinking & eating %d times.....\n", id, i);

	pthread_exit(NULL);
}

int main() {
	pthread_t	tid[NUM_MEN];
	int			i, id[NUM_MEN];

	srand(0x8888);

	for (i = 0 ; i < NUM_MEN ; i++)  {
		if (sem_init(&ChopStick[i], 0, 1) < 0)  {
			perror("sem_init");
			exit(1);
		}
		id[i] = i;
	}

	for (i = 0 ; i < NUM_MEN ; i++)  {
		if (pthread_create(&tid[i], NULL, (void *)DiningPhilosopher, 
				(void *)&id[i]) < 0)  {
			perror("pthread_create");
			exit(1);
		}
	}

	for (i = 0 ; i < NUM_MEN ; i++)  {
		if (pthread_join(tid[i], NULL) < 0)  {
			perror("pthread_join");
			exit(1);
		}
	}

	for (i = 0 ; i < NUM_MEN ; i++)  {
		if (sem_destroy(&ChopStick[i]) < 0)  {
			perror("sem_destroy");
		}
	}
	return 0;
}
