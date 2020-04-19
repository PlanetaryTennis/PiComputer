#include<stdio.h>
#include<dirent.h>
#include<sys/types.h>
#include<errno.h>
#include<string.h>
#include<mpi.h>

const int maxSize = 786976;

int found = 0;
int locations[786976];
int character[786976];

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
	FILE *fp;
	fp = fopen(directory, "r");
	int l = strlen(sKey)-1;
	char c;
	int tracker = 0;
	int matchers = 0;
	for(int i = 0;i < l;i++){
		c = getcaseless(fgetc(fp));
		if(feof(fp)){
			fclose(fp);
			return;
		}
		tracker++;
	}
	while(++tracker > 0){
		c = getcaseless(fgetc(fp));
//		printf("%c == %c\n",c, sKey[matchers]);
		if(feof(fp)){
			fclose(fp);
			return;
		}
		if(c == sKey[matchers]){
			matchers++;
		}else matchers = 0;
		if(matchers == l){
			add(location,tracker-l);
			matchers = 0;
		}
	}
	fclose(fp);
	printf("YoU cAn'T uSe PrInT sTaTmEnTs To DeBuG!\n");
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
						printf("Reading at %s\n",name);
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
}

void writefile(char* sKey){
	FILE *fp;
	fp = fopen("./outPut.txt", "w+");
	fprintf(fp,"%i matches found for %s in database.\n", found, sKey);
	for(int i = 1;i < found;i++){
		fputs(":---:\n", fp);
		fprintf(fp,"File Name: %s\n",library[locations[i]]);
		fprintf(fp,"Match at character: %i\n",character[i]);
	}
	fclose(fp);
}

void save(char* sKey){
	printf("Waiting\n");
	MPI_Barrier(MPI_COMM_WORLD);
	int bufsize = found;
	int charbuf[found];
	int locbuf[found];
	int v = 1;
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	for(int s = size-1;s > 0;s--){
		MPI_Barrier(MPI_COMM_WORLD);
		printf("%i\n",s);
		if(rank == s)for(int i = 0;i < found;i++){
			charbuf[i] = character[i];
			locbuf[i] = locations[i];
		}
		MPI_Bcast(&bufsize,1,MPI_INT,s,MPI_COMM_WORLD);
		if(bufsize != 0){
			printf("passing directory %i.\n",s);
			MPI_Bcast(&charbuf,bufsize,MPI_INT,s,MPI_COMM_WORLD);
			printf("passing locations %i.\n",v);
//			MPI_Bcast(&locbuf,bufsize,MPI_INT,s,MPI_COMM_WORLD);
			if(rank == 0){
				for(int i = 1;i < bufsize;i++){
					add(locbuf[i],charbuf[i]);
				}
			}
		}
		bufsize = found;
	}
	printf("going to write\n");
	if(rank == 0){
		writefile(sKey);
		printf("wrote\n");
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
		keycopy[i] = getcaseless(sKey[i]);
		i++;
	}
	keycopy[i] = '\0';
	sKey = keycopy;
        list(sKey);
	save(sKey);
	MPI_Barrier(MPI_COMM_WORLD);
	printf("Error is at the end. :(\n");
	MPI_Finalize();
	return 0;
}
