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

#define NUM_CLIENTS 5

typedef struct
{
    int choosing[NUM_CLIENTS];
    int number[NUM_CLIENTS];
} Ticket;


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

//APPLICATION
void sellTicket(Ticket* t, sem_t* sem) {
    down(sem);
    t->number = t->number + 1;
    up(sem);
}

int buyTicket(Ticket* t, sem_t* sem, int id) {
    t->choosing[id] = 1;
    t->number[id] = 
    }
}

Ticket* createSharedMemory() {
    int fd, data_size = sizeof(Ticket);
    Ticket* t;
    
    //Creates shared memory area
    if ((fd = shm_open("/ex12", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR)) < 0)
        perror("Failed to create shared memory");
    //Defines memory size
    if (ftruncate (fd, data_size) < 0)
        perror("Failed to adjust memory size");
    //Maps the area in an address
    t = (Ticket *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (t == NULL) {
        perror("Failed to map the area");
        exit(0);
    }
}