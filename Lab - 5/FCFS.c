// FCFS CPU SCHEDULING ALGORITHM
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>

#define THREADS 10

struct q{
	int *F, *R;
	int *queue;
}q;

WQ enQ(pthread_t tid, WQ R, WQ F){
	WQ temp = (WQ)malloc(sizeof(struct waitingQ));
	if(F == R){
		R = temp;
		R->tid = tid;
		R->next = NULL;
		F = R;
	}
	else{
		R->next = temp;
		R = R->next;
		R->tid = tid;
		R->next = NULL;
	}
	return R;
}

WQ deQ(WQ R, WQ F){
	WQ temp = F;
	if(F == R){
		F = R = NULL;
	}
	else{
		F = F->next;
	}
	temp->next = NULL;
	return temp;
}

int main(){
	q.F = -1, q.R = -1;
	pthread_t tid[5] = {0, 1, 2, 3, 4};
	for(int i=0; i<5; i++){
		enQ(tid[i], q);
	}
	for(int i=0; i<5; i++){
		int *temp = deQ(R, F);
		printf("thread id : %d\n", (int)temp->tid);
	}
	return 0;
}
