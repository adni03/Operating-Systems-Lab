//IMPLEMENTATION OF KILL COMMAND
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<signal.h>

int main(int argc, char **args){
	if(argc < 2){
		printf("usage: ./kill PID\n");
		return -1;
	}
	kill(atoi(args[1]), SIGKILL);
	return 0;
}
