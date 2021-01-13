//BASIC PROGRAM TO SWITCH BETWEEN 2 CONTEXTS
#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>
#define STACK_SIZE 16348

static ucontext_t uctx_main, uctx_func1, uctx_func2;

static void func1(void){
	printf("func1: started\n");
	printf("odd numbers\n");
	int i=13579;
	printf("i: %d\n", i);
	printf("func1: swapcontext(&uctx_func1, &uctx_func2)\n");
	if(swapcontext(&uctx_func1, &uctx_func2) == -1) perror("With error : ");
	printf("i: %d\n", i);
	printf("func1: returning\n");
}

static void func2(void){
	printf("func2: started\n");
	printf("even numbers\n");
	int i=24680;
	printf("i: %d\n", i);
	printf("func2: swapcontext(&uctx_func2, &uctx_func1)\n");
	if(swapcontext(&uctx_func2, &uctx_func1) == -1) perror("With error : ");
	printf("i: %d\n", i);
	printf("func2: returning\n");
}

int main(int argc, char* argv[]){
	char func1_stack[STACK_SIZE];
	char func2_stack[STACK_SIZE];
	
	if(getcontext(&uctx_func1) == -1){
		perror("With error : ");
	}
	uctx_func1.uc_stack.ss_sp = func1_stack;
	uctx_func1.uc_stack.ss_size = sizeof(func1_stack);
	uctx_func1.uc_link = &uctx_main;
	makecontext(&uctx_func1, func1, 0);
	
	if(getcontext(&uctx_func2) == -1){
		perror("With error : ");
	}
	uctx_func2.uc_stack.ss_sp = func2_stack;
	uctx_func2.uc_stack.ss_size = sizeof(func2_stack);
	uctx_func2.uc_link = &uctx_func1;
	makecontext(&uctx_func2, func2, 0);
	
	printf("main: swapcontext(&uctx_main, &uctx_func2)\n");
	if(swapcontext(&uctx_main, &uctx_func2) == -1) perror("With error : ");
	
	printf("main: exiting\n");
	exit(EXIT_SUCCESS);
}
