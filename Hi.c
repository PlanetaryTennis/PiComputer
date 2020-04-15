#include<stdio.h>
#include<dirent.h>
#include<sys/types.h>
#include<errno.h>
#include<string.h>

int isFile(char* name){
	printf("Please get here.\n");
	DIR* directory = opendir(name);
	if(directory != NULL){
		closedir(directory);
		return 0;
	}
	if(errno == ENOTDIR){
		return 1;
	}
	return -1;
}

char getcaseless(char in){
	char out;
	if(out > 64&&out < 91) out = out + 32;
	return out;
}

void DataFinder(char* sKey,char fPath[]){
	printf("Got here");
	FILE *domain;
	domain = fopen(fPath,'r');
	printf("Opened file.");
	int length = sizeof(sKey);
	printf("Found key length");
	int backPointer;
	char* stack = "";
	for(int i = 0;i < length;i++){
		stack += getcaseless(fgetc(domain));
	}
	printf("%s\n",stack);
	fclose(domain);
}

void list(char* sKey){
	DIR *dir;
	char* name;
	char directory[128];
	struct dirent *ent;
	if ((dir = opendir ("./data/")) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			name = ent->d_name;
			if(strcmp(name,".")&&strcmp(name,"..")){
				strncpy(directory,"./data/",sizeof(directory));
				strncat(directory, name, (sizeof(directory) - strlen(directory)) );
//				name = directory;
				printf("%s\n", directory);				
				if(isFile(name) == 1){
					printf("Hi");
					DataFinder(sKey,directory);
				}
			}
		}
		closedir (dir);
	} else {
		return;
	}
}

int main(int argc,char** argv){
	char* sKey = argv[1];
	printf("%s\n",sKey);
	list(sKey);//,"./data/testrange.txt");
	return 0;
}
