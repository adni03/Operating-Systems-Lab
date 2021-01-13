// BOUNDED BUFFER SYNCHRONIZATION PROBLEM
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/sem.h>
#include<sys/shm.h>
 
#define n 10
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

struct shmem{
	int buffer[n];
	int in, out;
};

void print(struct shmem* shm){
	printf("\n");
	for(int i=0; i<n; i++){
		printf("%d ", shm->buffer[i]);
	}
}

void *producer(void *arg){
	struct shmem* shm = (struct shmem*) arg;
	shm = (struct shmem*)shmat(shmid, NULL, 0);
	while(1){
		sleep(2);
		printf("\nProducing item..");
		p.sem_num = 0;
		semop(semid, &p, 1);
		if((shm->in+1)%n == shm->out){
			p.sem_num = 2;
			semop(semid, &p, 1);
		}
		shm->buffer[shm->in] = 1;
		shm->in = (shm->in+1)%n;
		print(shm);
		printf("\nPlacing in buffer..");
		v.sem_num = 0;
		semop(semid, &v, 1);
		v.sem_num = 2;
		semop(semid, &v, 1);
	}
}

void *consumer(void *arg){
	struct shmem* shm = (struct shmem*) arg;
	shm = (struct shmem*)shmat(shmid, NULL, 0);
	while(1){
		sleep(2);
		printf("\nConsuming item..");
		p.sem_num = 0;
		semop(semid, &p, 1);
		if(shm->in == shm->out){
			p.sem_num = 2;
			semop(semid, &p, 1);
		}
		shm->buffer[shm->out] = 0;
		print(shm);
		shm->buffer[shm->out] = -1;
		shm->out = (shm->out + 1)%n;
		printf("\nRemoving from buffer..");
		v.sem_num = 0;
		semop(semid, &v, 1);
		v.sem_num = 3;
		semop(semid, &v, 1);
	}
}

/*
	semaphore 0 --> mutex
	semaphore 1 --> full
	semaphore 2 --> empty
*/	

int main(){
	pthread_t con, prod;
	
	/* initializing shared memory */
	shmid = shmget(key, sizeof(struct shmem), IPC_CREAT | 0660);
	shm = (struct shmem*)shmat(shmid, NULL, 0);
	shm->in = shm->out = 0;
	for(int i=0; i<n; i++){
		shm->buffer[i] = -1;
	}
	print(shm);
	/* done */
	
	/* initializing aforementioned semaphores */
	union semun u;
	semid = semget(KEY, 3, 0666 | IPC_CREAT);
	u.val = 1;
	semctl(semid, 0, SETVAL, u);
	u.val = 0;
	semctl(semid, 1, SETVAL, u);
	u.val = n;
	semctl(semid, 2, SETVAL, u);
	/* done */
	
	pthread_create(&prod, NULL, producer, (void *) shm);
	pthread_create(&con, NULL, consumer, (void *) shm);
	pthread_join(con, NULL);
	pthread_join(prod, NULL);
	return 0;
}
