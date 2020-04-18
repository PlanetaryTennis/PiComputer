EXECS=Search
MPICC?=mpicc

all: ${EXECS}

Search: Search2.c
	${MPICC} -o Search Search2.c

clean:
	rm -f ${EXECS}
