#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include<sys/wait.h>

#define ARR_SIZE 1000000

typedef struct {
    int number;
} Number;

int generate_random_number() {
    //generates random number 1-5
    int upper = 5;
    int lower = 1;
    return (rand() % (upper - lower + 1)) + lower;
}

int main (void) {
    int fd, data_size = sizeof(Number);
    pid_t pid;
    int start, end;
    Number* n;

    //seed random number generator
    srand(time(NULL));
    
    //Creates shared memory area
    if ((fd = shm_open("/ex06", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR)) < 0)
        perror("Failed to create shared memory");
    //Defines memory size
    if (ftruncate (fd, data_size) < 0)
        perror("Failed to adjust memory size");
    //Maps the area in an address
    n = (Number *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (n == NULL) {
        perror("Failed to map the area");
        exit(0);
    }

    start = clock();

    for(int i = 0; i < ARR_SIZE; i++) {
        //Write number to shared memory
        n->number = generate_random_number();
        
        //Assigns a child to access shared memory and read the number
        if ((pid = fork()) > 0) {
            //Accesses shared memory area
            if ((fd = shm_open("/ex06", O_RDWR, S_IRUSR|S_IWUSR)) < 0)
                perror("Failed to create shared memory");
            //Defines memory size
            if (ftruncate (fd, data_size) < 0)
                perror("Failed to adjust memory size");
            //Maps the area in an address
            n = (Number*) mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
            if (n == NULL) {
                perror("Failed to map the area");
                exit(0);
            }

            //Reads number
            n->number;

            exit(0);
        }
    }

    while (wait(NULL) != -1);

    end = clock();
    printf("Time elapsed = %d", end - start);
}