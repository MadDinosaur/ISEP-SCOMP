SOURCES = ex02_reader.c
OBJFILES = ex02_reader.o
EXEC = ex02_reader

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