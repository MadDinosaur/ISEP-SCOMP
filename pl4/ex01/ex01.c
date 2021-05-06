//INCOMPLETO

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

#define NUM_PROCESSES 8
#define BUFFER_SIZE 200

pid_t createChildProcesses() {
    pid_t pid;
    for (int i = 0; i < NUM_PROCESSES; i++) {
        pid = fork();
        if (pid < 0)
            perror("Failed to create child.");
        if (pid > 0) {
            //printf("Child created\n");
            return pid;
        }
    }
}

void readNumbers(char* buf, sem_t* sem) {
    FILE *fp;
    sem_wait(sem);
    fp = fopen("Numbers.txt","r");
    fgets(buf, BUFFER_SIZE, fp);
    fclose(fp);
    //printf("Process is blocking access to Numbers file\n");
    sem_post(sem);
    //printf("Process released access to Numbers file\n");
}

void writeNumbers(char* buf, sem_t* sem) {
    FILE *fp;
    sem_wait(sem);
    fp = fopen("Output.txt","a");
    fputs(buf, fp);
    fclose(fp);
    //printf("Process is blocking access to Output file\n");
    sem_post(sem);
    //printf("Process released access to Output file\n");
}

int main (void) {
    pid_t pid;
    int status;
    char buf[BUFFER_SIZE];
    sem_t* sem_read, *sem_write;
    if ((sem_read = sem_open("read", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
     if ((sem_write = sem_open("write", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }

    
    pid = createChildProcesses();

    if (pid == 0) {
        for (int i = 0; i < NUM_PROCESSES; i++) {
            pid_t child_pid = wait(&status);
        }
        sem_unlink("read");
        sem_unlink("write");
    }
    else {
       readNumbers(buf, sem_read);
       writeNumbers(buf, sem_write);
       exit(0);
    }
            
    
    return 0;
}