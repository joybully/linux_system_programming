#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void calSum(int* sum) {
	int total_sum = 0;//sumation
	for(int i=0;i<50;i++){
		total_sum += sum[i];//add from index0 to index49
	}
	printf("%d\n",total_sum);
	pthread_exit(NULL);
}

int main() {
	pthread_t       tid1, tid2;//thread number
        
	int sum1[50] = {0,};
	int sum2[50] = {0,};
    for (int i = 0; i < 50; i++) {
        sum1[i] = i + 1;
	}
    for (int i = 0; i < 50; i++) {
        sum2[i] = i + 51;
	}
    if (pthread_create(&tid1,NULL,(void*)calSum,(void*)sum1)<0)  {
		//create thread, using void pointer to execute function calSum
        perror("pthread_create");
        exit(1);
    }
    if (pthread_create(&tid2, NULL, (void *)calSum, (void *)sum2) < 0)  {
		//creat thread, using void pointer to execute function calSum
        perror("pthread_create");
        exit(1);
    }
    if (pthread_join(tid1,NULL)<0)  { 
		//join the thread
        perror("pthread_join");
        exit(1);
    }
    if (pthread_join(tid2, NULL) < 0)  {
		//join the thread
        perror("pthread_join");
        exit(1);
    } 
    return 0;
}

