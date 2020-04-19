#include<stdio.h>
#include<dirent.h>
#include<sys/types.h>
#include<errno.h>
#include<string.h>
#include<mpi.h>

const int maxSize = 255;

int found = 0;
int locations[255];
int character[255];

char library[255][255];

int rank;
int size;

int isFile(char* name){
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
	char out = in;
	if(out > 64&&out < 91) out = out + 32;
	return out;
}

void add(int location,int placed){
	found++;
	locations[found] = location;
	character[found] = placed;
}

void DataFinder(char* sKey,char* directory,int location){
//	printf("Looking for \"%s\" in %s.\n",sKey,directory);
	add(location,32);
}

void list(char* sKey){
	DIR *dir;
	char* name;
	char directory[128];
	struct dirent *ent;
	int location = 0;
	if ((dir = opendir ("./data/")) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			if(location%size == rank||rank == 0){
				name = ent->d_name;
				if(strcmp(name,".")&&strcmp(name,"..")){
					strncpy(directory,"./data/",sizeof(directory));
					strncat(directory, name, (sizeof(directory) - strlen(directory)) );
					name = directory;
					if(isFile(name) == 1&&(rank != 0||location%size == rank)){
						DataFinder(sKey,name,location);
					}
					if(rank == 0){
						strncpy(library[location],name,sizeof(library[255]));
					}
				}
			}
			location++;
		}
		closedir (dir);
	} else {
		return;
	}
	if(rank == 0){
		printf("%s\n",library[0]);
	}
}

void writefile(char* sKey){
	FILE *fp;
	fp = fopen("./outPut.txt", "w+");
	for(int i = 1;i < found;i++){
		fputs(":---:\n", fp);
		fprintf(fp,"File Name: %s\n",library[locations[i]]);
		fprintf(fp,"Match at Character: %i\n",character[i]);
	}
	printf("Hi\n");
	fclose(fp);
}

void save(char* sKey){
        printf("Process with id %i is waiting.\n",rank);
	MPI_Barrier(MPI_COMM_WORLD);
	int bufsize = found;
	int charbuf[found];
	int locbuf[found];
	for(int s = size-1;s > 0;s--){
		if(rank == s)for(int i = 0;i < found;i++){
			charbuf[i] = character[i];
			locbuf[i] = locations[i];
		}
		MPI_Bcast(&bufsize,1,MPI_INT,s,MPI_COMM_WORLD);
		MPI_Bcast(&charbuf,bufsize,MPI_INT,s,MPI_COMM_WORLD);
		MPI_Bcast(&locbuf,bufsize,MPI_INT,s,MPI_COMM_WORLD);
		if(rank == 0){
//			printf("%i\n",bufsize);
			for(int i = 1;i < bufsize;i++){
				printf("%i\n%s\n",charbuf[i],library[locbuf[i]]);
				add(locbuf[i],charbuf[i]);
			}
		}
		bufsize = found;
	}
	if(rank == 0){
		printf("made it.\n",rank);
		writefile(sKey);
	}
}

int main(int argc,char** argv){
	MPI_Init(NULL,NULL);
	char* sKey = argv[1];
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	char keycopy[128];
	int i = 0;
	while(sKey[i]!='\0'){
		keycopy[i] = getcaseless(sKey[i++]);
	}
	keycopy[i+1] = '\0';
	sKey = keycopy;
        printf("Process with id %i is looking for key word \"%s\".\n",rank, sKey);
	list(sKey);
	save(sKey);
	printf("Out?\n");
	MPI_Finalize();
	return 0;
}
