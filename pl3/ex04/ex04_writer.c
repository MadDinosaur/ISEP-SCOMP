#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define ARR_SIZE 100

typedef struct {
    char chars[ARR_SIZE];
} Array;


int main (void) {
    int fd, data_size = sizeof(Array);
    char str[100];
    Array* a;
    
    //Creates shared memory area
    if ((fd = shm_open("/ex04", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR)) < 0)
        perror("Failed to create shared memory");
    //Defines memory size
    if (ftruncate (fd, data_size) < 0)
        perror("Failed to adjust memory size");
    //Maps the area in an address
    a = (Array *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (a == NULL) {
        perror("Failed to map the area");
        exit(0);
    }

    //Generates random chars and writes them to memory
    for (int i = 0; i < ARR_SIZE; i++) {
        char randomletter = 'A' + (random() % 26);
        a->chars[i] = randomletter;
    }
}