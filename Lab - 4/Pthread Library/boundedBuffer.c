// BOUNDED BUFFER SYNCHRONIZATION PROBLEM
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
 
#define n 10

int buffer[n], in = -1, out = -1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;

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
		pthread_mutex_lock(&mutex);
		if((in+1)%n == out){
			pthread_cond_wait(&empty, &mutex);
		}
		buffer[in] = 1;
		print();
		printf("\nPlacing in buffer..");
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&full);
	}
}

void *consumer(){
	while(1){
		sleep(2);
		printf("\nConsuming item..");
		pthread_mutex_lock(&mutex);
		if(in == out){
			pthread_cond_wait(&full, &mutex);
		}
		buffer[out] = 0;
		out = (out + 1)%n;	
		print();
		printf("\nRemoving from buffer..");
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&empty);
	}
}

int main(){
	pthread_t con, prod;
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
