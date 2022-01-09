#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 16
#define NUM_KEYS 8000
#define NUMBERS 5
#define LOTTERY_MIN 1
#define LOTTERY_MAX 49

//Lottery keys
typedef struct {
    int key[NUMBERS];
} Key;
Key keys[NUM_KEYS];
//Matrix containing statistics 
//(lines go from 1-49, columns have the number of times each number is drawn)
int statistics[LOTTERY_MAX - LOTTERY_MIN + 2][2];

pthread_mutex_t mutex;


int random_number(int lower, int upper){
    return (rand() % (upper - lower + 1)) + lower;
}

void populate_database() {
    printf("Populating database...\n");
    int i;
    for(i = 0; i < NUM_KEYS; i++)
        for(int j = 0; j < NUMBERS; j++)
            keys[i].key[j] = random_number(LOTTERY_MIN, LOTTERY_MAX);
    printf("Database complete with %d registers\n", i);
}

void populate_statistics() {
    printf("Populating statistics...\n");
    for(int i = LOTTERY_MIN; i <= LOTTERY_MAX; i++) {
        statistics[i][0] = i;
        statistics[i][1] = 0;
    }
    printf("Statistics ready to be calculated\n");
}

void increment_statistics(int num) {
    pthread_mutex_lock(&mutex);
    int current = statistics[num - LOTTERY_MIN][1];
    statistics[num - LOTTERY_MIN][1] = current + 1;
    pthread_mutex_unlock(&mutex);
}

void* thread_generate_statistics(void* arg){
    int index = *(int*)arg;
    int end = index + NUM_KEYS/THREADS;
    printf("Thread analysing database from %d to %d...\n", index, end);

    //Reads each number from each key in interval
    for (int i = index; i < end; i++)
        for (int j = 0; j < NUMBERS; j++)
            increment_statistics(keys[i].key[j]);
        
    //Ends thread
    pthread_exit((void*)NULL);
}

int main() {
    //Initialize mutex
    pthread_mutex_init(&mutex, NULL);

    //Seed random number
    srand ( time(NULL) );

    //FILL DATABASE AND SETUP STATISTICS
    populate_database();
    populate_statistics();
    
    //CREATE THREADS TO ACCOUNT KEYS
    pthread_t threads[THREADS];
    int args[THREADS];

    for (int i = 0; i < THREADS; i++){
        args[i] = i * NUM_KEYS/THREADS;

        if(pthread_create(&threads[i], NULL, thread_generate_statistics, (void*)&args[i]))
            perror("Error creating thread.");
    }
    //Wait for all calculations to be finished before proceeding
    for (int i = 0; i < THREADS; i++)
        if (pthread_join(threads[i], NULL))
            perror("Failed to join thread");
    //Destroy mutex
    pthread_mutex_destroy(&mutex);


    //PRINT RESULT STATISTICS
    printf("Final Statistics\n");
    for (int line = 0; line < LOTTERY_MAX - LOTTERY_MIN + 2; line++) {
        for(int col = 0; col < 2; col++)
            printf("%d     ", statistics[line][col]);
        printf("\n");
    }


    return 0;
}