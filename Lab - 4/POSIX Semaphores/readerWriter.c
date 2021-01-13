//READER AND WRITER SYNCHRONIZATION PROBLEM
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

sem_t r, w;
int readcnt = 0;
int re[5] = {0, 1, 2, 3, 4};
int wr[3] = {0, 1, 2};

void *writer(void *p){
	int *i = (int *) p;
	while(1){
		sem_wait(&w);
		printf("\nWriter %d writing to file", *i);
		sem_post(&w);
		sleep(1);
	}	
}

void *reader(void *p){
	int *i = (int *) p;
	while(1){
		sem_wait(&r);
		readcnt++;
		printf("\nReader count : %d", readcnt);
		if(readcnt == 1){
			printf("\nWriters blocked");
			sem_wait(&w);
		}
		sem_post(&r);
		printf("\nReader %d reading from the file", *i);
		sem_wait(&r);
		readcnt--;
		printf("\nReader %d finished reading", *i);
		printf("\nReader count : %d", readcnt);
		if(readcnt == 0){
			printf("\nWriters unblocked");
			sem_post(&w);
		}
		sem_post(&r);
		sleep(1);
	}
}

int main(){
	pthread_t readers[5], writes[3];
	sem_init(&r, 0, 1);
	sem_init(&w, 0, 1);
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
