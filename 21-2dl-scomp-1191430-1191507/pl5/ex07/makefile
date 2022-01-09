SOURCES = ex07.c
OBJFILES = ex07.o
EXEC = ex07

.SUFFIXES: .c .o

.c .o:
	gcc -Wall -g -c $<

${EXEC}: ${OBJFILES}
	gcc -Wall -g -o ${EXEC} ${OBJFILES} -lpthread -lrt

${OBJFILES}: ${SOURCES}

run: ${EXEC}
	./${EXEC}

clean:
	rm -f ${OBJFILES} ${EXEC}