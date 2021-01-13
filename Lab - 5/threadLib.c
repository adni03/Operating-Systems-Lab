// implement own thread library
#include<malloc.h>
#include<ucontext.h>
#include<stdio.h>
#include<sys/time.h>

#define MAXTHREAD 20
#define STACK_SIZE 32767

typedef struct thread_t{
	int thread_id;
	ucontext_t thread_context;
	struct thread_t* next_thread;
	struct thread_t* prev_thread;	
} thread_t;

thread_t thread_table[MAXTHREAD];
thread_t* ready_q = NULL, *current_thread = NULL;

void threadFunction(void *arg){
	int i, thrno;
	thrno = *((int*)arg);
	for(; ;){
		printf("This is child thread : %d\n", thrno);
		for(i=0; i<0x100000; i++);
	}
}

thread_t* thread_create(void (*threadFunction)(void), void *arg){
	int i;
	thread_t *thr;
	static int thread_no = 1;
	
	for(i=0; i<MAXTHREAD; i++){
		if(thread_table[i].thread_id == 0) break;
	}
	
	if(i >= MAXTHREAD) return NULL;
	thr = &thread_table[i];
	getcontext(&(thr->thread_context));
	
	thr->thread_context.uc_link = 0;
	thr->thread_context.uc_stack.ss_sp = malloc(STACK_SIZE);
	thr->thread_context.uc_stack.ss_size = STACK_SIZE;
	
	printf("Creating child fiber\n");
	makecontext(&(thr->thread_context), threadFunction, 1, arg);
	
}

int main(){
	thread_t *t1, *t2;
	int t1no = 1, t2no = 2, j;
	t1 = thread_create(threadFunction, (void*)&t1no);
	t2 = thread_create(threadFunction, (void*)&t2no);
	/*for(; ;){
		printf("This is main thread\n");
		for(j=0; j<0x200000; j++);
	}*/
	printf("Main thread\n");
}
