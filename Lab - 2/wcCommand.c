//IMPLEMENTATION OF wc COMMAND
#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
int main(int argc, char **args){
	int newLine=0, words=0, chars=0, flag=0, i = 1;
	int tNewLine=0, tWords=0, tChars=0;
	char buffer[1];
	while(i < argc){
		int fd = open(args[i], O_RDONLY);
		words=newLine=chars=0;
		if(fd == -1){
			printf("File couldnt be opened");
			perror("With error");
			return 0;
		}
		while(read(fd, buffer, 1)){
			chars++;
			flag = 0;
			if(buffer[0] == '\n'){
				newLine++;
				words++;
				flag=1;
			}
			if(buffer[0] == ' '){
				if(!flag) words++;
			}
		}
		tNewLine += newLine;
		tWords += words;
		tChars += chars;
		printf("%d\t%d\t%d\t%s\n", newLine, words, chars, args[i]);
		++i;	
	}
	printf("%d\t%d\t%d\t%s\n", tNewLine, tWords, tChars, "total");
	return 0;
}
