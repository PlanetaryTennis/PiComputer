#include<stdio.h>
#include<dirent.h>
#include<sys/types.h>
#include<errno.h>
#include<string.h>
#include<vector>
#include<mpi.h>

int found;
std::vector<char*> filename;
std::vector<int> line;
std::vector<int> character;

int rank;
int size;

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

void list(char* sKey){
	DIR *dir;
	char* name;
	char directory[128];
	struct dirent *ent;
	int location = 0;
	if ((dir = opendir ("./data/")) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			if(location%size == rank){
				name = ent->d_name;
				if(strcmp(name,".")&&strcmp(name,"..")){
					strncpy(directory,"./data/",sizeof(directory));
					strncat(directory, name, (sizeof(directory) - strlen(directory)) );
//					name = directory;
					printf("%s\n", directory);
//					if(isFile(name) == 1){
//						printf("Hi");
//						DataFinder(sKey,directory);
//					}
				}
			}
			location++;
		}
		closedir (dir);
	} else {
		return;
	}
}

void save(){
	MPI_Barrier(MPI_COMM_WORLD);
	looklist* buffer;
	int bufsize = found;
	MPI_DataType llist;
	MPI_Type_creat_struct(1, ,&llist);
	for(int s = size;s > 0;s++){
		
		MPI_Bcast(buffer,bufsize,,s,MPI_COMM_WORLD);
		if(rank == 0){
			add(buffer,looklist);
		}
	}
}

int main(int argc,char** argv){
	MPI_Init(NULL,NULL);
	char* sKey = argv[1];
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
        printf("Process with id %i is looking for key word %s.\n",rank, sKey);
	list(sKey);
	save();
	MPI_Finalize();
	return 0;
}
