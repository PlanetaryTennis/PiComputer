EXECS=Search
MPICC?=mpicc

all: ${EXECS}

Search: Search.c
	${MPICC} -o Search Search.c

clean:
	rm -f ${EXECS}
