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
sem_t mutex, self[n];

void test(int i){
	sleep(2);
	if((state[left] != eating && state[right] != eating) && state[i] == hungry){
		state[i] = eating;
		printf("\nPhilosopher %d is eating with chopsticks %d and %d", i, right, i);
		sem_post(&self[i]);
	}
}

void putdown(int i){
	sleep(2);
	sem_wait(&mutex);
	state[i] = thinking;
	test(left);
	test(right);
	sem_post(&mutex);
}

void pickup(int i){
	sleep(2);
	sem_wait(&mutex);
	state[i] = hungry;
	printf("\nPhilosopher %d is hungry..", i);
	test(i);
	sem_post(&mutex);
	if(state[i] != eating){
		sem_wait(&self[i]);
	}
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
	sem_init(&mutex, 0, 1);
	for(int i=0; i<n; i++){
		sem_init(&self[i], 0, 1);
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
