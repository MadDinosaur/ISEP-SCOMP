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
#include <semaphore.h>
#include <string.h>

#define NUM_PROCESSES 3
#define BUFFER_SIZE 200
#define MAX_RECORDS 100

typedef struct {
    char record[MAX_RECORDS][BUFFER_SIZE];
    int num_records;
} Records;

//SEMAPHORES
void down(sem_t *sem) {
    if (sem_wait(sem) == -1) {
        perror("Error at sem_wait().");
        exit(0);
    }
}

void up(sem_t *sem) {
    if (sem_post(sem) == -1) {
        perror("Error at sem_post().");
        exit(0);
    }
}

void consult_all(sem_t* sem) {
    //Accesses shared memory area
    int fd, data_size = sizeof(Records);
    Records *r;
    
    //Accesses shared memory area
    if ((fd = shm_open("/ex10", O_RDWR, S_IRUSR|S_IWUSR)) < 0)
        perror("Failed to create shared memory");
    //Defines memory size
    if (ftruncate (fd, data_size) < 0)
        perror("Failed to adjust memory size");
    //Maps the area in an address
    r = (Records*) mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
    if (r == NULL) {
        perror("Failed to map the area");
        exit(0);
    }

    char buf[BUFFER_SIZE];
    
    down(sem);
    for (int i = 0; i < r->num_records; i++) {
        printf("%s\n", r->record[i]);
    }
    up(sem);
}

int main(void) {
    sem_t *sem_file;

    sem_unlink("semaforo_fd");

    //Creates semaphore for shared memory access
    if ((sem_file = sem_open("semaforo_fd", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }

    consult_all(sem_file);

    sem_unlink("semaforo_fd");
    
    return 0;
}