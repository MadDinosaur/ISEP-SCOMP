SOURCES = ex12.c
OBJFILES = ex12.o
EXEC = ex12

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