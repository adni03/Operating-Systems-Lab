//IMPLEMENTATION OF cat COMMAND
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc, char **args){
	int i=1;
	while(i < argc){
		int fd = open(args[i], O_RDONLY);
		char buffer[1];
		if(fd < 0){
			return 0;
		}
		while(read(fd, buffer, 1)){
			printf("%s", buffer);
		}
		i++;
	}
	return 0;
}
