#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define ARR_SIZE 100

typedef struct {
    char chars[ARR_SIZE];
} Array;

int main(void) {
    int fd, data_size = sizeof(Array), sum = 0;
    Array *a;
    
    //Accesses shared memory area
    if ((fd = shm_open("/ex04", O_RDWR, S_IRUSR|S_IWUSR)) < 0)
        perror("Failed to create shared memory");
    //Defines memory size
    if (ftruncate (fd, data_size) < 0)
        perror("Failed to adjust memory size");
    //Maps the area in an address
    a = (Array*) mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
    if (a == NULL) {
        perror("Failed to map the area");
        exit(0);
    }

    for (int i = 0; i < ARR_SIZE; i++) {
        sum += a->chars[i];
    }

    printf("Average is: %d\n", sum/ARR_SIZE);

    if(munmap(a, data_size)<0){
        perror("No munmap()");
        exit(0);
    }

    if(close(fd)<0){
        perror("No close()");
        exit(0);
    }

    if(shm_unlink("/ex04")<0){
        perror("No unlink()");
        exit(1);
    }
}