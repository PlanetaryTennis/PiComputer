#include<stdio.h>
#include<dirent.h>

char getcaseless(char in){
	char out = in;
	if(out > 64&&out < 91) out = out + 32;
	return out;
}

void DataFinder(char* sKey,char* fPath){
	printf("test");
	FILE *domain;
	printf("Got here");
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
	printf("test");
	struct dirent *de;
	DIR *dr = opendir("./data");
	if (dr == NULL){
		printf("Could not open current directory" );
		return;
	}
	int size = 0;
	char* name = "";
	printf("test");
	while ((de = readdir(dr)) != NULL){
		size++;
		name = "./data/";
		strcat(name, &(de->d_name));
		printf("%s\n",name);
		DataFinder(sKey,name);
		name = "";
	}
	closedir(dr);
}

int main(int argc,char** argv){
	char* sKey = argv[1];
	printf("%s\n",sKey);
	printf("%c\n",getcaseless('A'));
	return 0;
}
