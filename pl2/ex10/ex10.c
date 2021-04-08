#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

int generate_random_number() {
    //generates random number 1-5
    int upper = 5;
    int lower = 1;
    return (rand() % (upper - lower + 1)) + lower;
}

int main (void) {
    int fd1[2], fd2[2];
    pid_t pid;

    int bet_target, bet, result, pipe_status;
    int credit = 20;

    //creates parent(writer) - child(reader) pipe
    if(pipe(fd1) == -1) {
        perror("Pipe 1 failed");
        return 1;
    }

    //creates parent(reader) - child(writer) pipe
    if(pipe(fd2) == -1) {
        perror("Pipe 2 failed");
        return 1;
    }

    pid = fork();
    //PARENT
    if (pid > 0) {
        //seed random number generator
        srand(getpid());

        //closes read descriptor on pipe 1 - unused by parent
        close(fd1[0]);
        //closes write descriptor on pipe 2 - unused by parent
        close(fd2[1]);
        
        while (credit > 0) {
            bet_target = generate_random_number();
            printf("Parent generated number %d.\n", bet_target);
    
            //writes to pipe
            result = credit > 0;
            write(fd1[1], &result, BUFFER_SIZE);
            //DEBUGGING
            //printf("Parent sent signal %d.\n", result);
            
            if (result) {
                do {
                    pipe_status = read(fd2[0], &bet, BUFFER_SIZE);
                } while (pipe_status <= 0);
                //DEBUGGING
                //printf("Parent received bet number %d.\n", bet);
                
                if (bet_target == bet)
                    credit += 10;
                else
                    credit -= 5;
                //DEBUGGING
                //printf("Parent updated credit to %d.\n", credit);
                write(fd1[1], &credit, BUFFER_SIZE);
            } else {
                wait(NULL);
            }
        }
        

        //closes write descriptor on pipe 1
        close(fd1[1]);
        //closes read descriptor on pipe 2
        close(fd2[0]);

    } else if (pid == 0) { //CHILD
        //seed random number generator
        srand(getpid());

        //closes write descriptor on pipe 1 - unused by child
        close(fd1[1]);
        //closes read descriptor on pipe 2 - unused by child
        close(fd2[0]);
        
        while (credit > 0) {
            //reads from pipe
            do {
                pipe_status = read(fd1[0], &result, BUFFER_SIZE);
            } while (pipe_status <= 0);
            //on a 0 - terminates
            if (result == 0)
                exit(0);

            //on a 1 - makes a bet and prints resulting credit
            bet = generate_random_number();
            printf("Child generated bet number %d.\n", bet);
            write(fd2[1], &bet, BUFFER_SIZE);
            
            do {
                pipe_status = read(fd1[0], &credit, BUFFER_SIZE);
            } while (pipe_status <= 0);

            printf("Current credit = %d.\n", credit);
        }
        //closes read descriptor on pipe 1
        close(fd1[0]);
        //closes write descriptor on pipe 2
        close(fd2[1]);
    } else {
        return 1;
    }
return 0;
}