#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>
#include <time.h>

#define ARR_SIZE 1000000
#define BUFFER_SIZE 4

int generate_random_number() {
    //generates random number 1-5
    int upper = 5;
    int lower = 1;
    return (rand() % (upper - lower + 1)) + lower;
}

int main (void) {
    int fd[2], start, end, num;
    pid_t pid;
    int arr[ARR_SIZE];

    //seed random number generator
    srand(time(NULL));

    //parent creates pipe
    if(pipe(fd) == -1) {
        perror("Pipe failed");
        return 1;
    }

    pid = fork();
    if (pid == 0) {
        start = clock();

        //closes read descriptor - unused by parent
        close(fd[0]);
        for (int i = 0; i < ARR_SIZE; i++) {
            write(fd[1], arr[i], BUFFER_SIZE);
        }
        //closes write descriptor
        close(fd[1]);

        while (wait(NULL) != -1);

        end = clock();
        printf("Time elapsed = %d", end - start);
    } else if (pid > 0) {
        //closes write descriptor - unused by child
        close(fd[1]);
        //reads from pipe
        while (read(fd[0], num, BUFFER_SIZE) > 0);
        //closes read descriptor
        close(fd[0]);
    } else {
        return 1;
    }
    return 0;
}