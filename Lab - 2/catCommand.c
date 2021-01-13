//IMPLEMENTATION OF cat COMMAND
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc, char **args){
	int fd = open(args[1], O_RDONLY);
	char buffer[1];
	if(fd < 0){
		printf("Couldnt open file");
		perror("With error");
		return 0;
	}
	while(read(fd, buffer, 1)){
		printf("%s", buffer);
	}
	return 0;
}
