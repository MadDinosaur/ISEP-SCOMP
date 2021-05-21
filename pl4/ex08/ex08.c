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

#define NUM_PROCESSES 1

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

int main (void) {
    pid_t pid;
    int status;
    sem_t* sem_s;
    sem_t* sem_c;
    
    sem_unlink("semaforo_s");
    sem_unlink("semaforo_c");


    if ((sem_s = sem_open("semaforo_s", O_CREAT | O_EXCL, 0644, 2)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    if ((sem_c = sem_open("semaforo_c", O_CREAT | O_EXCL, 0644, 2)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    
    pid = createChildProcesses();

    if (pid == 0) {
        for (int i = 0; i < 5; i++) {
            down(sem_s);
            printf("S\n");
            up(sem_c);
        }

        for (int i = 0; i < NUM_PROCESSES; i++) {
            pid_t child_pid = wait(&status);
        }
        printf("\n");
        sem_unlink("semaforo");
    }
    else {
       for (int i = 0; i < 5; i++) {
           down(sem_c);
           printf("C\n");
           up(sem_s);
       }
    }
  
    return 0;
}
