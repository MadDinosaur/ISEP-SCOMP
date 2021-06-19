#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 5
#define SIZE 1000

int data[SIZE];
int result[SIZE];

volatile int cnt = 0;
pthread_mutex_t result_mutex;
pthread_mutex_t print_mutex;
pthread_cond_t* cond = NULL;
pthread_t threads[THREADS];
pthread_t print_flags[THREADS];

int random_number(int lower, int upper){
    return (rand() % (upper - lower + 1)) + lower;
}

void populate_array() {
    printf("Populating data array...\n");
    int i;
    for(i = 0; i < SIZE; i++)
        data[i] = random_number(0, SIZE);
    printf("Data complete with %d registers\n", i);
}

void* thread_calculate(void* arg){
    int index = *(int*)arg;
    int end = index + SIZE/THREADS;
    printf("Thread calculating results from index %d to %d...\n", index, end);

    //CALCULATING AND SAVING
    for (int i = index; i < end; i++) {
        int result_num = data[i] * 10 + 2;

        //Entering critical section
        pthread_mutex_lock(&result_mutex);
        result[i] = result_num;
        pthread_mutex_unlock(&result_mutex);
    }

    //PRINTING
    int turn = index/(SIZE/THREADS);
 
    //Entering critical section
    pthread_mutex_lock(&print_mutex);

    //cnt is used to determne which thread should enter into critical section(printf() statement)
    if (turn != cnt) {
        //Puts all threads except one in waiting state
        pthread_cond_wait(&cond[turn], &print_mutex);
    }
    
    //Prints partial results
    for (int i = index; i < end; i++)
        printf("%d: %d\n", i, result[i]);

    //Determines which thread needs to be scheduled now
    if (cnt < THREADS - 1)
        cnt++;
    else
        cnt = 0;

    //Wakes up next thread
    pthread_cond_signal(&cond[cnt]);
    pthread_mutex_unlock(&print_mutex);
    
        
    //Ends thread
    pthread_exit((void*)NULL);
}

int main() {
    //Initialize mutex
    pthread_mutex_init(&result_mutex, NULL);
    pthread_mutex_init(&print_mutex, NULL);
    //Allocate memory to cond (conditional variable)
    cond = (pthread_cond_t*)malloc(sizeof(pthread_cond_t) * THREADS);
    //Initialize cond (conditional variable)
    for (int i = 0; i < THREADS; i++)
        if (pthread_cond_init(&cond[i], NULL) != 0)
            perror("pthread_cond_init() error");

    //Seed random number
    srand ( time(NULL) );

    //FILL DATA ARRAY
    populate_array();
    
    //CREATE THREADS TO PERFORM CALCULATIONS
    int args[THREADS];

    for (int i = 0; i < THREADS; i++){
        args[i] = i * SIZE/THREADS;

        if(pthread_create(&threads[i], NULL, thread_calculate, (void*)&args[i]))
            perror("Error creating thread.");
    }
    //Wait for all calculations to be finished before proceeding
    for (int i = 0; i < THREADS; i++)
        if (pthread_join(threads[i], NULL))
            perror("Failed to join thread");
    //Destroy mutex
    pthread_mutex_destroy(&result_mutex);
    pthread_mutex_destroy(&print_mutex);

    return 0;
}