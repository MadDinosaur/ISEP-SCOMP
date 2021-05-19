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

void insert(sem_t* sem, char* number, char* name, char* addr) {
    int fd, data_size = sizeof(Records);
    Records* r;
     //Creates shared memory area
    if ((fd = shm_open("/ex10", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR)) < 0)
        perror("Failed to create shared memory");
    //Defines memory size
    if (ftruncate (fd, data_size) < 0)
        perror("Failed to adjust memory size");
    //Maps the area in an address
    r = (Records *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (r == NULL) {
        perror("Failed to map the area");
        exit(0);
    }

    //Build text line
    char buf[BUFFER_SIZE] = "";
    strcat(buf, "No. ");
    strcat(buf, number);
    strcat(buf, " - ");
    strcat(buf, name);
    strcat(buf, " - ");
    strcat(buf, addr);
    strcat(buf, "\n");
    printf("%s\n", buf);

    //Writes to memory
    down(sem);
    strcpy(r->record[r->num_records], buf);
    r->num_records = r->num_records + 1;
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

    //Request input to insert
    char number[BUFFER_SIZE/3];
    char name[BUFFER_SIZE/3];
    char address[BUFFER_SIZE/3];

    printf("Insert identification number:\n");
    scanf("%s", &number);
    printf("Insert name:\n");
    scanf("%s", &name);
    printf("Insert address:\n");
    scanf("%s", &address);

    insert(sem_file, number, name, address);

    sem_unlink("semaforo_fd");   

    return 0;
}