// BOUNDED BUFFER SYNCHRONIZATION PROBLEM
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
 
#define n 10

int buffer[n], in = -1, out = -1;
sem_t mutex, empty, full;

void print(){
	printf("\n");
	for(int i=0; i<n; i++){
		printf("%d ", buffer[i]);
	}
}

void *producer(){
	while(1){
		sleep(2);
		printf("\nProducing item..");
		sem_wait(&empty);
		sem_wait(&mutex);
		if(in == out) in = out = 0;
		else in = (in + 1)%n;	
		buffer[in] = 1;
		print();
		printf("\nPlacing in buffer..");
		sem_post(&mutex);
		sem_post(&full);
	}
}

void *consumer(){
	while(1){
		sleep(2);
		printf("\nConsuming item..");
		sem_wait(&full);
		sem_wait(&mutex);
		buffer[out] = 0;
		out = (out + 1)%n;
		if(in == out) in = out = 0;	
		print();
		printf("\nRemoving from buffer..");
		sem_post(&mutex);
		sem_post(&empty);
	}
}

int main(){
	pthread_t con, prod;
	sem_init(&mutex, 0, 1);
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, n);
	for(int i=0; i<n; i++){
		buffer[i] = -1;
	}
	print();
	pthread_create(&prod, NULL, producer, NULL);
	pthread_create(&con, NULL, consumer, NULL);
	pthread_join(con, NULL);
	pthread_join(prod, NULL);
	return 0;
}
