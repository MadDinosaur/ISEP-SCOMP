SOURCES = ex08.c
OBJFILES = ex08.o
EXEC = ex08

.SUFFIXES: .c .o

.c .o:
	gcc -Wall -g -c $< -lrt

${EXEC}: ${OBJFILES}
	gcc -Wall -g -o ${EXEC} ${OBJFILES} -lrt

${OBJFILES}: ${SOURCES}

run: ${EXEC}
	./${EXEC}

clean:
	rm -f ${OBJFILES} ${EXEC}