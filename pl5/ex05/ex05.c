#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 3
#define NUM_CUSTOMERS 1000

int balances[NUM_CUSTOMERS];
int lowest_balance;
int highest_balance;
int average_balance;

int random_number(int lower, int upper){
    return (rand() % (upper - lower + 1)) + lower;
}

void populateArray() {
    for(int i = 0; i < NUM_CUSTOMERS; i++)
        balances[i] = random_number(0, 5000);
}

void* thread_searchLowest(void* arg){
    int lowest = -1;

    for(int i = 0; i < NUM_CUSTOMERS; i++)
        if(lowest == -1 || balances[i] < lowest)
            lowest = balances[i];

    lowest_balance = lowest;
    //Ends thread
    pthread_exit((void*)NULL);
}

void* thread_searchHighest(void* arg){
    int highest = -1;

    for(int i = 0; i < NUM_CUSTOMERS; i++)
        if(balances[i] > highest)
            highest = balances[i];

    highest_balance = highest;
    //Ends thread
    pthread_exit((void*)NULL);
}

void* thread_calcAverage(void* arg){
    int average = 0;

    for(int i = 0; i < NUM_CUSTOMERS; i++)
        average = average + balances[i];

    average = average/NUM_CUSTOMERS;
    average_balance = average;
    //Ends thread
    pthread_exit((void*)NULL);
}

int main() {
    //Seed random number
    srand ( time(NULL) );

    //Fill balances array with random numbers
    populateArray();
    
    //CALCULATE STATISTICS
    pthread_t threads[THREADS];

    if(pthread_create(&threads[0], NULL, thread_searchLowest, (void*)NULL))
        perror("Error creating thread.");
    if(pthread_create(&threads[1], NULL, thread_searchHighest, (void*)NULL))
        perror("Error creating thread.");
    if(pthread_create(&threads[2], NULL, thread_calcAverage, (void*)NULL))
        perror("Error creating thread.");

    //Wait for all calculations to be finished before proceeding
    for (int i = 0; i < THREADS; i++)
        if (pthread_join(threads[i], NULL))
            perror("Failed to join thread");
    
    //PRINT RESULT
    printf("Lowest balance = %d\n", lowest_balance);
    printf("Highest balance = %d\n", highest_balance);
    printf("Average balance = %d\n", average_balance);

    return 0;
}