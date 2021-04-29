SOURCES = ex06_shared.c
OBJFILES = ex06_shared.o
EXEC = ex06_shared

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