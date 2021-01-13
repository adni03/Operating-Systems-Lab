//SLEEPING BARBER PROBLEM
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define n 5
int freeSeats = n;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cust = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t barb = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t doneCut = PTHREAD_MUTEX_INITIALIZER;

void *barber(){
	while(1){
		sleep(1);
		pthread_mutex_lock(&cust);
		pthread_mutex_unlock(&barb);
		pthread_mutex_lock(&mutex);
		freeSeats = freeSeats + 1;
		pthread_mutex_unlock(&mutex);
		pthread_mutex_unlock(&doneCut);
		printf("\nBarber giving haircut..\tfreeseats : %d", freeSeats);
	}
}

void *customer(){
	while(1){
		sleep(1);
		pthread_mutex_lock(&mutex);
		if(freeSeats > 0){
			freeSeats = freeSeats - 1;
			printf("\nCustomer arrived..\tfreeseats : %d", freeSeats);
			pthread_mutex_unlock(&mutex);
			pthread_mutex_unlock(&cust);
			pthread_mutex_lock(&barb);
			pthread_mutex_lock(&doneCut);
		}
		else{
			printf("\nCustomer kicked");
			pthread_mutex_unlock(&mutex);
		}
	}
}

int main(){
	pthread_t c[5], b;
	for(int i=0; i<5; i++){
		pthread_create(&c[i], NULL, customer, NULL);
	}
	pthread_create(&b, NULL, barber, NULL);
	for(int i=0; i<5; i++){	
		pthread_join(c[i], NULL);
	}
	pthread_join(b, NULL);
	return 0;
}
