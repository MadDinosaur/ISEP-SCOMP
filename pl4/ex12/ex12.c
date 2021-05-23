#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include<sys/wait.h>
#include <semaphore.h>

#define NUM_TICKETS 5

typedef struct {
    int ticket_num;
} Ticket;

typedef struct {
    int fd;
    int data_size;
} SHM;

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

sem_t* createSemaphore() {
    sem_t* sem;
    if ((sem = sem_open("semaforo", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    return sem;
}

sem_t* accessSemaphore() {
    sem_t* sem;
    if ((sem = sem_open("semaforo", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    return sem;
}

void closeSemaphore() {
    sem_unlink("semaforo");
}
//CHILDREN
pid_t generateChildren(int num_children) {
    pid_t pid;

    for (int i = 0; i < num_children; i++) {
        if ((pid = fork()) > 0) return pid;
    }

    return 0;
}

//SHM
Ticket* createSharedMemory(SHM* shm) {
    shm->data_size = sizeof(Ticket);
    Ticket* t;
    if ((shm->fd = shm_open("/ex12", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR)) < 0)
        perror("Failed to create shared memory");
    //Defines memory size
    if (ftruncate (shm->fd, shm->data_size) < 0)
        perror("Failed to adjust memory size");
    //Maps the area in an address
    t = (Ticket *) mmap(NULL, shm->data_size, PROT_READ|PROT_WRITE, MAP_SHARED, shm->fd, 0);
    if (t == NULL) {
        perror("Failed to map the area");
        exit(1);
    }
    return t;
}

Ticket* accessSharedMemory() {
    int fd, data_size = sizeof(Ticket);
    Ticket* t;

    if ((fd = shm_open("/ex12", O_RDWR, S_IRUSR|S_IWUSR)) < 0)
        perror("Failed to create shared memory");
    //Defines memory size
    if (ftruncate (fd, data_size) < 0)
        perror("Failed to adjust memory size");
    //Maps the area in an address
    t = (Ticket*) mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
    if (t == NULL) {
        perror("Failed to map the area");
        exit(1);
    }

    return t;
}

void closeSharedMemory(Ticket* t, SHM* shm) {
        while (wait(NULL)!= -1);
        
        if(munmap(t, shm->data_size)<0){
            perror("No munmap()");
            exit(1);
        }

        if(close(shm->fd)<0){
            perror("No close()");
            exit(1);
        }

        if(shm_unlink("/ex12")<0){
            perror("No unlink()");
            exit(1);
        }
}

int main(void) {
    SHM* shm;
    shm->data_size = -1;
    shm->fd = -1;
    
    sem_t* sem = createSemaphore();

    //Initializes tickets
    Ticket* t = createSharedMemory(shm);
    t->ticket_num = -1;
    
    //Generates clients
    pid_t client = generateChildren(NUM_TICKETS);
    if (client) {
        //This is the ticket buyer process
        sem = accessSemaphore();
        //Waits for a ticket to be available
        while(t->ticket_num == -1);
        //Blocks access to other clients and takes the ticket
        down(sem);
        printf("Customer no. %d has ticket no. %d\n", client, t->ticket_num);
        t->ticket_num = -1;
        up(sem);

        exit(0);
    } else {
        //This is the ticket seller process
        
        //Defines ticket numbers and order
        int tickets[NUM_TICKETS] = {3, 10, 2, 90, 6};
        int num_sold = 0;

        for(int i = 0; i < NUM_TICKETS; i++) {
            //Waits for a client to buy the ticket
            while(t->ticket_num != -1);
            //Blocks access to clients and writes the ticket number
            down(sem);
            t->ticket_num = tickets[num_sold];
            num_sold++;
            up(sem);
        }

        closeSharedMemory(t, shm);
        closeSemaphore(sem);
    }
}