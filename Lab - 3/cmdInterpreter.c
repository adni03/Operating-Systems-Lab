// COMMAND INTERPRETER WITH I/O REDIRECTION
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#define n 10
#define m 100

int length(char *command){
	int i=0, cnt=0;
	while(command[i] != '\0'){
		++i;
		++cnt;
	}
	return cnt;
}

void flush(char argv[n][m], int *argc, int *len){
	for(int i=0; i<10; i++){
		for(int j=0; j<100; j++){
			argv[i][j] = '\0';
		}
	}
	*argc = *len = 0;
}

int getChars(char command[50], char argv[n][m], int argc, int len){
	for(int i=0; i < (int)strlen(command); i++){
		if(command[i] == ' '){
			argv[argc][len] = '\0';
			len = 0;
			argc++;
		}
		else{
			argv[argc][len++] = command[i];
		}
	}
	return argc;
}

void lsCommand(char argv[n][m], int argc){
	int pid = fork();
	if(pid == 0){
		if(argc == 0) execlp("./ls", "", NULL);
		else if(!strcmp(argv[1], "-a")) execlp("./ls-a", "", NULL);
		else if(!strcmp(argv[1], "-l")) execlp("./ls-l", "", NULL);	
	}
	else wait(&pid);
}

void wcCommand(char argv[n][m], int argc){
	int pid = fork();
	if(pid == 0) execlp("./wc", argv[0], argv[1], argv[2], argv[3], argv[4], NULL);
	else wait(&pid);
}

void catCommand(char argv[n][m], int argc){
	int pid = fork();
	if(pid == 0) execlp("./cat", argv[0], argv[1], argv[2], NULL);
	else wait(&pid);
}

void rmCommand(char argv[n][m], int argc){
	int pid = fork();
	if(pid == 0) execlp("./rm", argv[0], argv[1], argv[2], NULL);
	else wait(&pid);
}

void killCommand(char argv[n][m], int argc){
	int pid = fork();
	if(pid == 0) execlp("./kill", argv[0], argv[1], argv[2], NULL);
	else wait(&pid);
}

void psCommand(char argv[n][m], int argc){
	int pid = fork();
	if(pid == 0) execlp("./ps", "", NULL);
	else wait(&pid);
}

/*void outputRedirection(char argv[n][m], int argc){
	if(!strcmp(argv[1], ">>")){
		int fd = open(argv[2], O_CREAT | O_WRONLY | O_APPEND);
		char words[1024];
		fgets(words, 1024, stdin);
		write(fd, words, sizeof(words));
		close(fd);
	}
	else if(!strcmp(argv[1], ">")){
		int fd = open(argv[2], O_CREAT | O_WRONLY);
		char words[1024];
		fgets(words, 1024, stdin);
		write(fd, words, sizeof(words));
		close(fd);
	}
}*/

void callCommands(char argv[n][m], int argc){
	if(!strcmp(argv[0], "ls")) lsCommand(argv, argc);
	else if(!strcmp(argv[0], "wc")) wcCommand(argv, argc);
	else if(!strcmp(argv[0], "cat")) catCommand(argv, argc);
	else if(!strcmp(argv[0], "rm")) rmCommand(argv, argc);
	else if(!strcmp(argv[0], "kill")) killCommand(argv, argc);
	else if(!strcmp(argv[0], "ps")) psCommand(argv, argc);
	//else if(argv[1][0] == '>') outputRedirection(argv, argc);
}

int main(){
	char command[50], argv[n][m];
	int argc = 0, len = 0, size = 64;
	while(1){
		printf("$ ");
		gets(command);
		len = argc = 0;
		//printf("%d", (int)strlen(command));
		if(strcmp(command, "exit") == 0) break;
		argc = getChars(command, argv, argc, len);
		callCommands(argv, argc);
		printf("\n");
	}
	return 0;
}
