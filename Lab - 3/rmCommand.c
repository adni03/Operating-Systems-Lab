//IMPLEMENTATION OF rm COMMAND
#include<stdio.h>
#include<string.h>
#include<dirent.h>
#include<fcntl.h>

void removeFiles(char* path){
	struct dirent* d;
	DIR *dir = opendir(path);
	while(d = readdir(dir)){
		printf("%s", d->d_name);
		if(d->d_name[0] != '.'){
			if(remove(d->d_name) != 0){
				removeFiles(d->d_name);
			}
		}
	}
	closedir(dir);
}

int main(int argc, char **argv){
	int i=1;
	for(int j=0; j<argc; j++){
		printf("%s\n", argv[j]);
	}
	if(argc >= 2 && strcmp(argv[1], "-i") == 0){
		for(int i=2; i<argc; i++){
			char ch;
			printf("Do you want to delete? Y/N \n");
			scanf("%c", &ch);
			if(ch == 'Y' || ch == 'y'){
				int status = remove(argv[i]);
				if(status == 0) printf("File deleted successfully");
			}
		}
	}
	else if(strcmp(argv[2], "-r") == 0){
		for(int i=3; i<argc; i++){
			if(remove(argv[i]) != 0){
				printf("\nCouldnt delete directory");
				removeFiles(argv[i]);
			}
		}
	}
	else{
		for(int i = 1; i<argc; i++){
			int st = remove(argv[i]);
		} 
	}
	return 0;
}
