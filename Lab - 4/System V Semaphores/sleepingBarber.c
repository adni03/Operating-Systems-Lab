//SLEEPING BARBER PROBLEM
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/sem.h>
#include<sys/shm.h>

#define n 5
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
	int freeSeats;
};

void *barber(){
	struct shmem* smh = (struct shmem*)shmat(shmid, NULL, 0);
	while(1){
		sleep(1);
		p.sem_num = 1;
		semop(semid, &p, 1);
		
		v.sem_num = 2;
		semop(semid, &v, 1);
		
		v.sem_num = 3;
		semop(semid, &v, 1);
		
		p.sem_num = 0;
		semop(semid, &p, 1);
		
		smh->freeSeats = smh->freeSeats + 1;
		
		v.sem_num = 0;
		semop(semid, &v, 1);
		
		printf("\nBarber giving haircut..\tfreeseats : %d", smh->freeSeats);
	}
}

void *customer(){
	struct shmem* smh = (struct shmem*)shmat(shmid, NULL, 0);
	while(1){
		sleep(1);
		p.sem_num = 0;
		semop(semid, &p, 1);
		
		if(smh->freeSeats > 0){
			smh->freeSeats = smh->freeSeats - 1;
			printf("\nCustomer arrived..\tfreeseats : %d", smh->freeSeats);
			v.sem_num = 0;
			semop(semid, &v, 1);
			
			v.sem_num = 1;
			semop(semid, &v, 1);
			
			p.sem_num = 2;
			semop(semid, &p, 1);

			p.sem_num = 3;
			semop(semid, &p, 1);			
		}
		else{
			printf("\nCustomer kicked");
			v.sem_num = 0;
			semop(semid, &v, 1);
		}
	}
}

/*
	semaphore 0 --> mutex
	semaphore 1 --> customer
	semaphore 2 --> barber
	semaphore 3 --> donecut
*/

int main(){
	pthread_t c[10], b;
	
	/* initializing shared memory */
	shmid = shmget(key, sizeof(struct shmem), IPC_CREAT | 0660);
	shm = (struct shmem*)shmat(shmid, NULL, 0);
	shm->freeSeats = n;
	/* done */
	
	/* initializing aforementioned semaphores */
	semid = semget(KEY, 3, 0666 | IPC_CREAT);
	union semun u;
	u.val = 1;
	semctl(semid, 0, SETVAL, u);
	u.val = 0;
	semctl(semid, 1, SETVAL, u);
	u.val = 0;
	semctl(semid, 2, SETVAL, u);
	u.val = 0;
	semctl(semid, 3, SETVAL, u);
	/* done */
	
	for(int i=0; i<10; i++){
		pthread_create(&c[i], NULL, customer, NULL);
	}
	pthread_create(&b, NULL, barber, NULL);
	for(int i=0; i<10; i++){	
		pthread_join(c[i], NULL);
	}
	pthread_join(b, NULL);
	return 0;
}
