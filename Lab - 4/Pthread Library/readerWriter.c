//READER AND WRITER SYNCHRONIZATION PROBLEM
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

pthread_mutex_t r = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t w = PTHREAD_MUTEX_INITIALIZER;
int readcnt = 0;
int re[5] = {0, 1, 2, 3, 4};
int wr[3] = {0, 1, 2};

void *writer(void *p){
	int *i = (int *) p;
	while(1){
		pthread_mutex_lock(&w);
		printf("\nWriter %d writing to file", *i);
		pthread_mutex_unlock(&w);
		sleep(1);
	}	
}

void *reader(void *p){
	int *i = (int *) p;
	while(1){
		pthread_mutex_lock(&r);
		readcnt++;
		printf("\nReader count : %d", readcnt);
		if(readcnt == 1){
			printf("\nWriters blocked");
			pthread_mutex_lock(&w);
		}
		pthread_mutex_unlock(&r);
		printf("\nReader %d reading from the file", *i);
		pthread_mutex_lock(&r);
		readcnt--;
		printf("\nReader %d finished reading", *i);
		printf("\nReader count : %d", readcnt);
		if(readcnt == 0){
			printf("\nWriters unblocked");
			pthread_mutex_unlock(&w);
		}
		pthread_mutex_unlock(&r);
		sleep(1);
	}
}

int main(){
	pthread_t readers[5], writes[3];
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
