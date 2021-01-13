//DINING PHILOSOPHER SYNCHRONIZATION PROBLEM
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define n 5
#define eating 2
#define hungry 1
#define thinking 0
#define right (i + 4) % n
#define left (i + 1) % n

int state[n];
int p[n] = {0, 1, 2, 3, 4};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t self[n] = PTHREAD_COND_INITIALIZER;

void test(int i){
	sleep(2);
	if((state[left] != eating && state[right] != eating) && state[i] == hungry){
		state[i] = eating;
		printf("\nPhilosopher %d is eating with chopsticks %d and %d", i, right, i);
		pthread_cond_signal(&self[i]);
	}
}

void putdown(int i){
	sleep(2);
	pthread_mutex_lock(&mutex);
	state[i] = thinking;
	test(left);
	test(right);
	pthread_mutex_unlock(&mutex);
}

void pickup(int i){
	sleep(2);
	pthread_mutex_lock(&mutex);
	state[i] = hungry;
	printf("\nPhilosopher %d is hungry..", i);
	test(i);
	pthread_mutex_unlock(&mutex);
	pthread_cond_wait(&self[i], &mutex);
}

void *runner(void *parm){
	int *num = (int *) parm;
	while(1){
		printf("\nPhilosopher %d wants to pick up chopsticks..", *num);
		pickup(*num);
		putdown(*num);
		printf("\nPhilosopher %d is putting down chopsticks..", *num);
	}
}

int main(){
	pthread_t thread_id[n];
	for(int i=0; i<n; i++) {
		state[i] = thinking;
	}
	for(int i=0; i<n; i++){
		pthread_create(&thread_id[i], NULL, runner, &p[i]);
	}
	for(int i=0; i<n; i++){
		pthread_join(thread_id[i], NULL);
	}
	return 0;
}
