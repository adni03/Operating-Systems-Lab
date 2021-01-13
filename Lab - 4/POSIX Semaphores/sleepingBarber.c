//SLEEPING BARBER PROBLEM
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define n 4
int freeSeats = n;
sem_t mutex, cust, barb;

void *barber(){
	while(1){
		sleep(1);
		sem_wait(&cust);
		sem_wait(&mutex);
		freeSeats = freeSeats + 1;
		sem_post(&mutex);
		sem_post(&barb);
		printf("\nBarber giving haircut..\tfreeseats : %d", freeSeats);
	}
}

void *customer(void *parm){
	int* num = (int *) parm;
	while(1){
		sleep(1);
		sem_wait(&mutex);
		if(freeSeats > 0){
			freeSeats = freeSeats - 1;
			printf("\nCustomer %d arrived..\tfreeseats : %d", *num, freeSeats);
			sem_post(&cust);
			sem_post(&mutex);
			sem_wait(&barb);
		}
		else{
			printf("\nCustomer %d kicked", *num);
			sem_post(&mutex);
		}
	}
}

int main(){
	pthread_t c[5], b;
	sem_init(&mutex, 0, 1);
	sem_init(&cust, 0, 0);
	sem_init(&barb, 0, 0);
	for(int i=0; i<5; i++){
		pthread_create(&c[i], NULL, customer, (void *) i);
  	}
	pthread_create(&b, NULL, barber, NULL);
	for(int i=0; i<5; i++){	
		pthread_join(c[i], NULL);
	}
	pthread_join(b, NULL);
	return 0;
}
