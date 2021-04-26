#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <time.h>

#define ARRAY_SIZE 10

void showArray(int begin, int end, int *vec){
    printf("Vec: ");

	for(int i=begin; i<end; i++)
	{
		printf("%d | ", vec[i]);
	}
    printf("\n");
}

typedef struct {
	int numbers[ARRAY_SIZE];
} ShmDataType;

int main() {
	ShmDataType *shmData;
	int fd,soma=0;

	if((fd = shm_open("/shmtest", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO)) <0)
		perror("Failed to create shared memory");

	if(ftruncate(fd, sizeof(ShmDataType))<0)
        perror("Failed to adjust memory size");

	shmData = (ShmDataType *)mmap(NULL, sizeof(ShmDataType), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (shmData == NULL) {
        perror("No mmap()");
        exit(0);
    }

	showArray(0, ARRAY_SIZE, shmData->numbers);
	int i;

	for(i = 0; i < ARRAY_SIZE; i++)
		soma += shmData->numbers[i];
	printf("\nAverage = %f", ((float)soma/ARRAY_SIZE));
	printf("\n");

	if(munmap(shmData, sizeof(ShmDataType))<0){
        perror("No munmap()");
        exit(0);
    }

	if(close(fd)<0){
        perror("No close()");
        exit(0);
    }

    if(shm_unlink("/shmtest")<0){
        perror("No unlink()");
        exit(1);
    }

	return 0;
}
