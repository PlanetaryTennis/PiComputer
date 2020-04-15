#include<stdio.h>
#include<dirent.h>

char getcaseless(char in){
	char out;
	if(out > 64&&out < 91) out = out + 32;
	return out;
}

void DataFinder(char* sKey,char* fPath){
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
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir ("./")) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			printf ("%s\n", ent->d_name);
		}
		closedir (dir);
	} else {
		return;
	}
}

int main(int argc,char** argv){
	MPI_Init(NULL, NULL);
	char* sKey = argv[1];
	printf("%s\n",sKey);
	list(sKey);//,"./data/testrange.txt");
	MPI_Finalize();
	return 0;
}
