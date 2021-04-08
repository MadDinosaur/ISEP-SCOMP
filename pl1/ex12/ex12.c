#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define N 6
int spawn_childs(int n) {
    int i, pid;
    for (i = 0; i < n; i++) {
        pid = fork();
        if (pid == 0) {
            return i + 1;
        }
    }
    return 0;
}


void main() {
    int status;

    int id = spawn_childs(N);
    if (id > 0) exit(id * 2);
    else {
        for (int j = 0; j < N; j++) {
            wait(&status);
            printf("Child process no. %d exited successfully.\n", WEXITSTATUS(status));
        }
    }
    exit(0);
}

