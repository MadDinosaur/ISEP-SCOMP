#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#define SIZE_NAME 50

typedef struct {
	char name[SIZE_NAME];
	int number;
} Student;

int main() {
	Student *s;
	int fd, size=sizeof(Student);

	if((fd = shm_open("/shmtest", O_CREAT | O_RDWR | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO)) <0)
		perror("Failed to create shared memory");

	if(ftruncate(fd, size)<0)
        perror("Failed to adjust memory size");

	s = (Student *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (s == NULL) {
        perror("No mmap()");
        exit(0);
    }

	printf("\nInsert the student's name: ");
	fgets(s->name, sizeof(s->name), stdin);

	fflush(stdin);

	printf("\nInsert the student's number: ");
	scanf("%d", &(s->number));
	printf("\n");

	if(munmap(s, size)<0){
        perror("No munmap()");
        exit(0);
    }

	if(close(fd)<0){
        perror("No close()");
        exit(0);
    }

	return 0;
}
