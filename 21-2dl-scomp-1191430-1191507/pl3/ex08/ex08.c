#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include<sys/wait.h>

#define NUM_OPERATIONS 10

typedef struct {
    int value;
} Number;

int main (void) {
    pid_t pid;
    int fd, data_size = sizeof(Number);
    Number* n;

    //Creates shared memory
    if ((fd = shm_open("/ex08", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR)) < 0)
        perror("Failed to create shared memory");
    //Defines memory size
    if (ftruncate (fd, data_size) < 0)
        perror("Failed to adjust memory size");
    //Maps the area in an address
    n = (Number *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (n == NULL) {
        perror("Failed to map the area");
        exit(1);
    }

    //Initializes value to 100
    n->value = 100;

    //Creates new process
    pid = fork();
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        //Increase the value
        n->value = n->value + 1;
        //Decrease the value
        n->value = n->value - 1;
    }
    printf("value = %d\n", n->value);
    //Closes shared memory area
    if (pid == 0) {
        if(munmap(n, data_size)<0){
            perror("No munmap()");
            exit(1);
        }

        if(close(fd)<0){
            perror("No close()");
            exit(1);
        }

        if(shm_unlink("/ex08")<0){
            perror("No unlink()");
            exit(1);
        }
    wait(NULL);
    }
}