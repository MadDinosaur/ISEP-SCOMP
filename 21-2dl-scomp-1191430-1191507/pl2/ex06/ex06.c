#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100
#define ARR_SIZE 1000
#define DIV 5

void initialize_array(int* array, int size) {
    int i;
    time_t t;
    srand((unsigned)time(&t));
    for (i = 0; i < size; i++) {
        array[i] = rand()%10000;
    }
}

int generate_children(int* vec1, int* vec2, int* fd) {
    int i;
    int tmp = 0;

    for (i = 0; i < DIV; i++) {    
        int p = fork();
        if (p < 0) exit(-1); //ERRO
        if (p == 0) {
            int j;
            for (j = 0; j < ARR_SIZE/DIV; j++) {
                //calculate sum
                tmp += vec1[j] + vec2[j];
            }
            //closes read descriptor - unused by child
            close(fd[0]);
            //writes to pipe
            write(fd[1], &tmp, BUFFER_SIZE);
            //closes write descriptor
            close(fd[1]);
            
            exit(0);
        }
        vec1 += ARR_SIZE/DIV;
        vec2 += ARR_SIZE/DIV;
    }
}
int main (void) {
    int fd[2];
    pid_t pid;

    int vec1[ARR_SIZE];
    int vec2[ARR_SIZE];

    initialize_array(vec1, ARR_SIZE);
    initialize_array(vec2, ARR_SIZE);

    //parent creates pipe
    if(pipe(fd) == -1) {
        perror("Pipe failed");
        return 1;
    }

    generate_children(vec1, vec2, fd);

    int partial_sum;
    int sum = 0;

    //closes write descriptor - unused by parent
    close(fd[1]);
    
    //waits for children
    int c;
    for (c = 0; c < DIV; c++)
        wait(NULL);

    //reads from pipe
    while (read(fd[0], &partial_sum, BUFFER_SIZE) > 0)
        sum += partial_sum;
    
    printf("Array sum is %d.\n", sum);
    //closes read descriptor
    close(fd[0]);

    return 0;
}