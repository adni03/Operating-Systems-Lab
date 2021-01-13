//IMPLEMENTATION OF wc COMMAND
#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char **args){
	printf("%s", args[1]);
	return 0;
	int newLine=0, words=0, chars=0, flag=0, i = 1, start = 1;
	int tNewLine=0, tWords=0, tChars=0;
	char buffer[1];
	if(argc >=2 && (strcmp(args[1], "-w") == 0 || strcmp(args[1], "-l") == 0 || strcmp(args[1], "-c") == 0)) start++;
	i = start;
	while(i < argc){
		int fd = open(args[i], O_RDONLY);
		words=newLine=chars=0;
		if(fd == -1){
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
		if(start == 1) printf("%d\t%d\t%d\t%s\n", newLine, words, chars, args[i]);
		else if(strcmp(args[1], "-w") == 0) printf("%d\t%s\n", words, args[i]);
		else if(strcmp(args[1], "-l") == 0) printf("%d\t%s\n", newLine, args[i]);
		else if(strcmp(args[1], "-c") == 0) printf("%d\t%s\n", chars, args[i]);
		++i;	
	}
	return 0;
}
