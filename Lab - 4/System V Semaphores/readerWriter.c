//READER AND WRITER SYNCHRONIZATION PROBLEM
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/sem.h>
#include<sys/shm.h>
#define KEY 123

int re[5] = {0, 1, 2, 3, 4};
int wr[3] = {0, 1, 2};

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

struct sembuf p = {0, -1, SEM_UNDO};
struct sembuf v = {0, 1, SEM_UNDO};
int semid, shmid, key;

struct shmem{
	int readcnt;
};

void *writer(void *arg){
	int *i = (int *) arg;
	struct shmem* smh = (struct shmem*)shmat(shmid, NULL, 0);
	while(1){
		p.sem_num = 1;
		semop(semid, &p, 1);
		
		printf("\nWriter %d writing to file", *i);
		
		v.sem_num = 1;
		semop(semid, &v, 1);
	}	
}

void *reader(void *arg){
	int *i = (int *) arg;
	struct shmem* smh = (struct shmem*)shmat(shmid, NULL, 0);
	while(1){
		p.sem_num = 0;
		semop(semid, &p, 1);
		
		smh->readcnt++;
		printf("\nReader count : %d", smh->readcnt);
		if(smh->readcnt == 1){
			p.sem_num = 1;
			semop(semid, &p, 1);
			printf("\nWriters blocked");
		}
		
		v.sem_num = 0;
		semop(semid, &v, 1);
		
		printf("\nReader %d reading from the file", *i);
		
		p.sem_num = 0;
		semop(semid, &p, 1);
		
		smh->readcnt--;
		printf("\nReader %d finished reading", *i);
		printf("\nReader count : %d", smh->readcnt);
		if(smh->readcnt == 0){
			v.sem_num = 1;
			semop(semid, &v, 1);
			printf("\nWriters unblocked");
		}
		
		v.sem_num = 0;
		semop(semid, &v, 1);
	}
}

/*
	semaphore 0 --> mutex
	semaphore 1 --> database
*/

int main(){
	pthread_t readers[5], writes[3];
	
	/* shared memory initialization */
	struct shmem* shm;
	shmid = shmget(key, sizeof(struct shmem), IPC_CREAT | 0660);
	shm = (struct shmem*)shmat(shmid, NULL, 0);
	shm->readcnt = 0;
	/*done*/
	
	/* semaphore initialization */
	semid = semget(KEY, 3, 0666 | IPC_CREAT);
	union semun u;
	u.val = 1;
	semctl(semid, 0, SETVAL, u);
	u.val = 1;
	semctl(semid, 1, SETVAL, u);
	/* done */
	
	for(int i=0; i<3; i++){
		pthread_create(&writes[i], NULL, writer, &wr[i]);
	}
	for(int i=0; i<5; i++){
		pthread_create(&readers[i], NULL, reader, &re[i]);
	}
	for(int i=0; i<5; i++){
		pthread_join(readers[i], NULL);
	}
	for(int i=0; i<3; i++){
		pthread_join(writes[i], NULL);
	}
	return 0;
}
