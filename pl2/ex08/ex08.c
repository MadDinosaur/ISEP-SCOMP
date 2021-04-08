//TODO: Fix round numbers...

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100
#define DIV 10

struct Message{
    char str[BUFFER_SIZE];
    int round_no;
};

void generate_children(int* fd) {
    int i;
    struct Message msg;

    for (i = 0; i < DIV; i++) {    
        int p = fork();
        if (p < 0) exit(-1); //ERRO
        if (p == 0) {
            //closes write descriptor - unused by child
            close(fd[1]);
            //tries to read from pipe
            if (read(fd[0], &msg, BUFFER_SIZE) > 0) {
                printf("%s, Round no. %d.\n", msg.str, msg.round_no);
                //closes write descriptor
                close(fd[1]);
                exit(msg.round_no);
            }
        }
    }
}
int main (void) {
    int fd[2];
    pid_t pid;
    int status;

    clock_t time;
    clock_t elapsed_time;

    struct Message msg;
    strcpy(msg.str, "Win");
    msg.round_no = 0;

    //parent creates pipe
    if(pipe(fd) == -1) {
        perror("Pipe failed");
        return 1;
    }

    generate_children(fd);

    //closes read descriptor - unused by parent
    close(fd[0]);

    time = clock()/CLOCKS_PER_SEC;
    while (msg.round_no < DIV) {
        elapsed_time = clock()/CLOCKS_PER_SEC;

        if(elapsed_time != time && elapsed_time % 2 == 0) {
            time = elapsed_time;
            
            msg.round_no = msg.round_no + 1;
            write(fd[1], &msg, BUFFER_SIZE);

            printf("%d\n", msg.round_no);
        }
    }

    //closes write descriptor
    close(fd[1]);
    
    //waits for children
    int c;
    for (c = 0; c < DIV; c++) {
        pid = wait(&status);
        printf("Child %d won round %d.\n", pid, WEXITSTATUS(status));
    }

    return 0;
}