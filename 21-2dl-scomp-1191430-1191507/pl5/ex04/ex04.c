#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define FILL_THREADS 2
#define CALC_THREADS 8
#define MATRICES 2
#define MATRIX_SIZE 8

int matrix1[MATRIX_SIZE][MATRIX_SIZE];
int matrix2[MATRIX_SIZE][MATRIX_SIZE];
int matrixResult[MATRIX_SIZE][MATRIX_SIZE];

int random_number(int lower, int upper){
    return (rand() % (upper - lower + 1)) + lower;
}

void* thread_populateMatrix(void* arg){
    int* matrix = (int*)arg;
    printf("Populating matrix...\n");

    for (int i = 0; i < MATRIX_SIZE*MATRIX_SIZE; i++)
        *(matrix + i) = random_number(0, 8);
 
    printf("Matrix filled!\n");
    //Ends thread
    pthread_exit((void*)NULL);
}

void* thread_multiplyMatrix(void* arg) {
    int line = *(int*) arg;
    printf("Calculating on line %d...\n", line);
    
    for (int col = 0; col < MATRIX_SIZE; col++) {
        int result = 0;
        //Multiplies line by col
        for(int j = 0; j < MATRIX_SIZE; j++)
            result = result + matrix1[line][j] * matrix2[j][col];

        //Writes calculation result to new matrix
        printf("Result on cell (%d, %d) is %d\n", line, col, result);
        matrixResult[line][col] = result;
    }
    //Ends thread
    pthread_exit((void*)NULL);
}

int main() {
    //Seed random number
    srand ( time(NULL) );

    //FILL TWO MATRICES
    pthread_t fill_threads[FILL_THREADS];
   
    if(pthread_create(&fill_threads[0], NULL, thread_populateMatrix, (void*)&matrix1[0][0]))
        perror("Error creating thread.");
    if(pthread_create(&fill_threads[1], NULL, thread_populateMatrix, (void*)&matrix2[0][0]))
        perror("Error creating thread.");
    //Wait for matrices to be filled before proceeding
    for (int i = 0; i < FILL_THREADS; i++)
        if (pthread_join(fill_threads[i], NULL))
            perror("Failed to join thread");
    
    //MULTIPLY MATRICES
    pthread_t calc_threads[CALC_THREADS];
    int args[CALC_THREADS];

    for (int i = 0; i < CALC_THREADS; i++){
        args[i] = i;

        if(pthread_create(&calc_threads[i], NULL, thread_multiplyMatrix, (void*)&args[i]))
            perror("Error creating thread.");
    }
    //Wait for all calculations to be finished before proceeding
    for (int i = 0; i < CALC_THREADS; i++)
        if (pthread_join(calc_threads[i], NULL))
            perror("Failed to join thread");
    
    //PRINT RESULT MATRIX
    printf("Result matrix is\n");
    for (int line = 0; line < MATRIX_SIZE; line++) {
        for(int col = 0; col < MATRIX_SIZE; col++)
            printf("%d     ", matrixResult[line][col]);
        printf("\n");
    }


    return 0;
}