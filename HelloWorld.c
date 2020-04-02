#include<mpi.h>
#include<stdio.h>

int main(int argc,char** argv){
	MPI_Init(NULL, NULL);
	char pname[MPI_MAX_PROCESSOR_NAME];
	int name;
	MPI_Get_processor_name(pname, &name);
	printf("Processor %s says \"Hello World!\" from core %d.\n",pname, sched_getcpu());
	MPI_Finalize();
	return 0;
}
