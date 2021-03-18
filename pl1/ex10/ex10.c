#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define ARR 2000
#define DIV 10
void initialize_array(int* array, int size) {
    int i;
    for (i = 0; i < size; i++) {
        array[i] = rand()/1000000;
    }
}

int generate_children(int* array, int n) {
    int i;
    for (i = 0; i < DIV; i++) {
        
        int p = fork();
        if (p < 0) exit(-1); //ERRO
        if (p == 0) {
            int j;
            for (j = 0; j < ARR/DIV; j++) {
                //DEBUGGING
                //printf("%d, %d\n", j, *(array + j));
                if (*(array + j) == n) {
                    //printf("index = %d\n", j);
                    exit(j);
                }
            }
            exit(255);
        }
        array += ARR/DIV;
    }
}

void main() {
    int array[ARR];
    int n = 424;
    int status;

    initialize_array(array, sizeof(array)/sizeof(int));
    //DEBUGGING
    /*int p;
    printf("array = ");
    for (p=0; p<ARR;p++) {
        printf("%d ",array[p]);
    }*/

    printf("n = %d\n", n);

    generate_children(array, n);

    int c;
    for (c = 0; c < DIV; c++) {
        wait(&status);
        if(WEXITSTATUS(status) != 255)
            printf("index = %d\n", WEXITSTATUS(status));
    }

    exit(0);
}

