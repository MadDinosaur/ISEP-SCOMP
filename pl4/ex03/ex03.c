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

#define NUM_PROCESSES 5

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

void writeText(sem_t* sem) {
    //Build text line
    char buf[35];
    snprintf(buf, 35, "I'm the father - with PID %d.\n", getpid()); // puts string into buffer

    //Opens file and writes
    FILE *fp;
    down(sem);
    fp = fopen("Output.txt","a");
    fputs(buf, fp);
    fclose(fp);
    up(sem);
    
    sleep(5);

    exit(0);
}

int main (void) {
    pid_t pid;
    int status;
    sem_t* sem;
    sem_unlink("semaforo");

     if ((sem = sem_open("semaforo", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }

    
    pid = createChildProcesses();

    if (pid == 0) {
        for (int i = 0; i < NUM_PROCESSES; i++) {
            pid_t child_pid = wait(&status);
        }
        sem_unlink("semaforo");
    }
    else {
       writeText(sem);
    }
            
    
    return 0;
}
