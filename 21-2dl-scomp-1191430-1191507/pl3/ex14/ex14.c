#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include<sys/wait.h>

#define BUFFER_SIZE 10
#define NUM_VALUES 30

typedef struct {
    int buffer[BUFFER_SIZE];
    int writing_to_buffer[BUFFER_SIZE]; //synchronization variable
} Buffer;

void initialize_array(int* arr, int value, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = value;
    }
}

int main (void) {
    pid_t pid;
    int fd, data_size = sizeof(Buffer);
    Buffer* b;

    //Creates shared memory area
    if ((fd = shm_open("/ex14", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR)) < 0)
        perror("Failed to create shared memory");
    //Defines memory size
    if (ftruncate (fd, data_size) < 0)
        perror("Failed to adjust memory size");
    //Maps the area in an address
    b = (Buffer *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (b == NULL) {
        perror("Failed to map the area");
        exit(1);
    }

    initialize_array(b->buffer, -1, BUFFER_SIZE);
    //DEBUG
    //printf("Buffer is initialized at %d\n", b->buffer[0]);

    if ((pid = fork()) > 0) {
        //This is the consumer

        //Accesses shared memory area
        if ((fd = shm_open("/ex14", O_RDWR, S_IRUSR|S_IWUSR)) < 0)
            perror("Failed to create shared memory");
        //Defines memory size
        if (ftruncate (fd, data_size) < 0)
            perror("Failed to adjust memory size");
        //Maps the area in an address
        b = (Buffer*) mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
        if (b == NULL) {
            perror("Failed to map the area");
            exit(1);
        }

        //Reads values from memory
        int j = 0;
        for (int i = 0; i < NUM_VALUES; i++) {
            if (j == BUFFER_SIZE) j = 0;
            //Waits for parent to write in index j
            while(b->buffer[j] == -1 || b->writing_to_buffer[j] == 1);

            //Reads and resets the value
            printf("Consumer reads %d\n", b->buffer[j]);
            b->buffer[j] = -1;

            j++;
        }

    } else {
        //This is the producer

        //Writes 30 values to memory
        int j = 0;
        for (int i = 0; i < NUM_VALUES; i++) {
            if (j == BUFFER_SIZE) j = 0;
            
            //DEBUG
            //printf("Parent initializing...\nBuffer is at %d\n", b->buffer[j]);
            
            //Waits until child has read the value on index j before overwriting
            while(b->buffer[j] != -1);
        
            b->writing_to_buffer[j] = 1;
            b->buffer[j] = i + 1;
            b->writing_to_buffer[j] = 0;
            j++;
        }

        //Waits for child and closes the shared memory
        wait(NULL);
        if(munmap(b, data_size)<0){
            perror("No munmap()");
            exit(1);
        }

        if(close(fd)<0){
            perror("No close()");
            exit(1);
        }

        if(shm_unlink("/ex14")<0){
            perror("No unlink()");
            exit(1);
        }
    }
    return 0;
}