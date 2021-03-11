#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void main() {
    int i;
    int status;

    for (i = 0; i < 4; i++) {
        if (fork() == 0) {
            sleep(1);
        }
    }

    printf("This is the end.\n");
}

void alineaA() {
    printf("The code will create 16 processes.");
}

void alineaB() {
    int i;
    int status;

    for (i = 0; i < 2; i++) {
        if (fork() == 0) {
            sleep(1);
        }
    }

    printf("This is the end.\n");
}

void alineaC() {
    int i;
    int status;

    for (i = 0; i < 2; i++) {
        int pid = fork();
        if (pid == 0) {
            sleep(1);
        } else if (pid % 2 == 0) {
            waitpid(pid, &status, 0);
        }
    }

    printf("This is the end.\n");
}

void alineaD() {
    int i;
    int status;
    int order = 0;

    for (i = 0; i < 2; i++) {
        int pid = fork();
        if (pid == 0) {
            order += 1;
            sleep(1);
        } else if (pid % 2 == 0) {
            waitpid(pid, &status, 0);
        }
    }
    printf("Creation order: %d\n", order);
    printf("This is the end.\n");
}