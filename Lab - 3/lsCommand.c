//IMPLEMENTATION OF ls COMMAND
#include<stdio.h>
#include<dirent.h>

int main(int argc,  char **args){
	struct dirent *de;
	DIR *dr = opendir(".");
	if(!dr){
		printf("Couldnt open directory");
		perror("With error");
		return 0;
	}
	while((de = readdir(dr))){
		if(de->d_name[0] != '.') {
			printf("%s\t", de->d_name);
		}
	}
	return 0;
}
