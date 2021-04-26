#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <time.h>

#define ARRAY_SIZE 10
#define MINIMUM 1
#define MAXIMUM 20

void populateArray(int n, int *vec){
    int i;

    time_t t;
    srand ((unsigned) time (&t));

    for (i = 0; i < n; i++)
    {
        vec[i] = rand () % MAXIMUM + MINIMUM;
    }
}

typedef struct {
	int numbers[ARRAY_SIZE];
} ShmDataType;

int main() {
	ShmDataType *shmData;
	int fd;

	if((fd = shm_open("/shmtest", O_CREAT | O_RDWR | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO)) <0)
		perror("Failed to create shared memory");

	if(ftruncate(fd, sizeof(ShmDataType))<0)
        perror("Failed to adjust memory size");

	shmData = (ShmDataType *)mmap(NULL, sizeof(ShmDataType), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (shmData == NULL) {
        perror("No mmap()");
        exit(0);
    }

	populateArray(ARRAY_SIZE, shmData->numbers);

	if(munmap(shmData, sizeof(ShmDataType))<0){
        perror("No munmap()");
        exit(0);
    }

	if(close(fd)<0){
        perror("No close()");
        exit(0);
    }

	return 0;
}
