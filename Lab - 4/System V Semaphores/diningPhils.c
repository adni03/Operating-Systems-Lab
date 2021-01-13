//DINING PHILOSOPHER SYNCHRONIZATION PROBLEM
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/sem.h>
#include<sys/shm.h>

#define n 5
#define eating 2
#define hungry 1
#define thinking 0
#define right (i + 4) % n
#define left (i + 1) % n
#define KEY 123

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

struct sembuf p = {0, -1, SEM_UNDO}; //wait
struct sembuf v = {0, 1, SEM_UNDO}; //signal
struct shmem* shm;
int semid, shmid, key;
int num[n] = {0, 1, 2, 3, 4};

struct shmem{
	int state[n];
};

void test(int i, struct shmem* smh){
	sleep(2);
	if((smh->state[left] != eating && smh->state[right] != eating) && smh->state[i] == hungry){
		smh->state[i] = eating;
		printf("\nPhilosopher %d is eating with chopsticks %d and %d", i, right, i);
		v.sem_num = 1+i;
		semop(semid, &v, 1);
	}
}

void putdown(int i, struct shmem* smh){
	sleep(2);
	p.sem_num = 0;
	semop(semid, &p, 1);
	
	smh->state[i] = thinking;
	test(left, smh);
	test(right, smh);
	
	v.sem_num = 0;
	semop(semid, &v, 1);
}

void pickup(int i, struct shmem* smh){
	sleep(2);
	p.sem_num = 0;
	semop(semid, &p, 1);
	
	smh->state[i] = hungry;
	printf("\nPhilosopher %d is hungry..", i);
	test(i, smh);
	
	v.sem_num = 0;
	semop(semid, &v, 1);
	p.sem_num = 1+i;
	semop(semid, &p, 1);
}

void *runner(void *parm){
	int *num = (int *) parm;
	struct shmem* smh = (struct shmem*)shmat(shmid, NULL, 0);
	while(1){
		printf("\nPhilosopher %d wants to pick up chopsticks..", *num);
		pickup(*num, smh);
		putdown(*num, smh);
		printf("\nPhilosopher %d is putting down chopsticks..", *num);
	}
}

/*
	semaphore 0 --> mutex
	semaphore 1 to 5 --> philosophers
*/

int main(){
	pthread_t thread_id[n];
	
	/* initializing shared memory */
	shmid = shmget(key, sizeof(struct shmem), IPC_CREAT | 0660);
	shm = (struct shmem*)shmat(shmid, NULL, 0);
	for(int i=0; i<n; i++) {
		shm->state[i] = thinking;
	}
	/* done */
	
	/* initializing aforementioned semaphores */
	union semun u;
	semid = semget(KEY, 6, 0666 | IPC_CREAT);
	u.val = 1;
	semctl(semid, 0, SETVAL, u);
	for(int i=0; i<n; i++){
		u.val = 0;
		semctl(semid, 1+i, SETVAL, u);
	}
	/* done */

	for(int i=0; i<n; i++){
		pthread_create(&thread_id[i], NULL, runner, &num[i]);
	}
	for(int i=0; i<n; i++){
		pthread_join(thread_id[i], NULL);
	}
	return 0;
}
