#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 5
#define ARRAY_SIZE 100
#define RANGE 10

int array[ARRAY_SIZE];
int number;

int randomNumber(int lower, int upper){
    srand ( time(NULL) );
    return (rand() % (upper - lower + 1)) + lower;
}

void populateArray(int n, int *vec){
    int i;
    time_t t;
    srand ((unsigned) time (&t));

    for (i = 0; i < n; i++)
        vec[i] = rand () % RANGE;
}

void* funcSearchNumberInArray(void* arg) {
    int *index = (int*) arg;
    int begin = (ARRAY_SIZE / THREADS) * *index;
    int end = (ARRAY_SIZE / THREADS) * (*index + 1);
    for(int i = begin; i < end; i++) {
        if(array[i] == number) {
            printf("The number %d is in the index: %d\n", number, i);
            pthread_exit((void*)index);
        }
    }
    pthread_exit(NULL);
}

int main() {
    int i;

    number = randomNumber(0, RANGE);
    printf("Number %d\n", number);
    
    populateArray(ARRAY_SIZE, array);

    pthread_t threads[THREADS];
    int indexes[THREADS];
    for(i = 0; i < THREADS; i++) {
        indexes[i] = i;
    }
   
    for(i = 0; i < THREADS; i++) {
        if(pthread_create(&threads[i], NULL, funcSearchNumberInArray, (void*)&indexes[i])) {
            perror("Error creating thread.");
        }
    }

    int* return_values[THREADS];
    for(i = 0; i < THREADS; i++) {
        void* return_val;
        pthread_join(threads[i], &return_val);
        return_values[i] = return_val;
    }

    for(i = 0; i < THREADS; i++) {
        if(return_values[i] != NULL) {
            printf("The number %d was found on thread %d.\n", number, *return_values[i]);
        }
    }

    return 0;
}