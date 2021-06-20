#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define FILTERING_THREADS 3
#define COMPUTING_THREADS 3
#define PRODUCTS 5
#define HYPERMARKETS 3
#define SIZE 100

typedef struct {
    int id_h;
    int id_p;
    int p;
} Product;

typedef struct {
    int id_h;
    int cost;
} LowestCost;

//General data
Product vec[SIZE];
LowestCost lowestCost;
//Hypermarket 1
Product vec1[SIZE];
volatile int vec1_products = 0;
//Hypermarket 2
Product vec2[SIZE];
volatile int vec2_products = 0;
//Hypermarket 3
Product vec3[SIZE];
volatile int vec3_products = 0;


pthread_mutex_t mutex_vec1;
pthread_mutex_t mutex_vec2;
pthread_mutex_t mutex_vec3;
pthread_mutex_t mutex_computing;
pthread_mutex_t mutex_cost;

pthread_cond_t cond_filtering;


int random_number(int lower, int upper){
    return (rand() % (upper - lower + 1)) + lower;
}

void populate_database() {
    printf("Populating database...\n");
    int i;
    for(i = 0; i < SIZE; i++) {
        vec[i].id_h = random_number(1, HYPERMARKETS);
        vec[i].id_p = random_number(1, PRODUCTS);
        vec[i].p = random_number(1, 50);
    }
    printf("Database complete with %d registers\n", i);
}

void initialize_array(int* array, int size) {
    for (int i = 0; i < size; i++)
        *(array + i) = 0;
} 

void copy_array(Product *a,Product *b,int n) {
    for(int i = 0; i < n; i++) 
        b[i] = a[i];
 }

void* thread_filtering(void* arg){
    int index = *(int*)arg;
    int end = index + SIZE/FILTERING_THREADS;
    printf("Thread analysing database from %d to %d...\n", index, end);

    //Reads each product from database in interval
    for (int i = index; i < end; i++) {
        //Hypermarket id
        int id_h = vec[i].id_h;

        if(id_h == 1) {
            pthread_mutex_lock(&mutex_vec1);
            vec1[vec1_products].id_h = vec[i].id_h;
            vec1[vec1_products].id_p = vec[i].id_p;
            vec1[vec1_products].p = vec[i].p;
            vec1_products++;
            pthread_mutex_unlock(&mutex_vec1);
        } else if(id_h == 2) {
            pthread_mutex_lock(&mutex_vec2);
            vec2[vec2_products].id_h = vec[i].id_h;
            vec2[vec2_products].id_p = vec[i].id_p;
            vec2[vec2_products].p = vec[i].p;
            vec2_products++;
            pthread_mutex_unlock(&mutex_vec2);
        } else if(id_h == 3) {
            pthread_mutex_lock(&mutex_vec3);
            vec3[vec3_products].id_h = vec[i].id_h;
            vec3[vec3_products].id_p = vec[i].id_p;
            vec3[vec3_products].p = vec[i].p;
            vec3_products++;
            pthread_mutex_unlock(&mutex_vec3);
        }
    }
    printf("Thread finished analysing database\n");
    //Ends thread
    pthread_exit((void*)NULL);
}

void* thread_computing(void* arg){
    int id_h = *(int*)arg;
    printf("Booted up thread for hypermarket %d\n", id_h);
    //Defines which hypermarket this thread is dedicated to
    Product vec_h[SIZE];
    int vec_h_size;
    if (id_h == 1) {
        copy_array(vec1, vec_h, vec1_products);
        vec_h_size = vec1_products;
    } else if (id_h == 2) {
        copy_array(vec2, vec_h, vec2_products);
        vec_h_size = vec2_products;
    } else if (id_h == 3) {
        copy_array(vec3, vec_h, vec3_products);
        vec_h_size = vec3_products;
    }
    printf("Hypermarket assigned\n");

    //Waits for filtering process
    pthread_mutex_lock(&mutex_computing);
    printf("Waiting on filtering threads...\n");
    pthread_cond_wait(&cond_filtering, &mutex_computing);
    pthread_mutex_unlock(&mutex_computing);
    printf("Ready to compute!\n");

    //Calculates average price of each product
    int avg[PRODUCTS];
    int qty[PRODUCTS];
    initialize_array(avg, PRODUCTS);
    initialize_array(qty, PRODUCTS);
    
    for (int i = 0; i < vec_h_size; i++) {
        int prod_index = vec_h[i].id_p - 1;
        avg[prod_index] = avg[prod_index] + vec_h[i].p;
        qty[prod_index] = qty[prod_index] + 1;
    }
    for (int j = 0; j < PRODUCTS; j++) {
        avg[j] = avg[j]/qty[j];
    }
    //Calculates the cost of the 5 products on the hypermarket
    int cost = 0;
    for (int k = 0; k < PRODUCTS; k++)
        cost = cost + avg[k];
    printf("Cost of products on hypermarket %d is %d\n", id_h, cost);

    //Checks global variable - lowestCost - and overwrites it if necessary
    pthread_mutex_lock(&mutex_cost);
    if(lowestCost.cost == -1 || lowestCost.cost > cost) {
        lowestCost.id_h = id_h;
        lowestCost.cost = cost;
    }
    pthread_mutex_unlock(&mutex_cost);

    printf("Thread finished computing\n");
    //Ends thread
    pthread_exit((void*)NULL);
}

int main() {
    //Initialize mutex
    pthread_mutex_init(&mutex_vec1, NULL);
    pthread_mutex_init(&mutex_vec2, NULL);
    pthread_mutex_init(&mutex_vec3, NULL);
    pthread_mutex_init(&mutex_cost, NULL);
    pthread_mutex_init(&mutex_computing, NULL);
    //Initialize condition variables
    pthread_cond_init(&cond_filtering, NULL);
    //Initialize business variables
    lowestCost.id_h = -1;
    lowestCost.cost = -1;

    //Seed random number
    srand ( time(NULL) );

    //FILL DATABASE AND SETUP STATISTICS
    populate_database();
    
    //FILTERING
    pthread_t filtering_threads[FILTERING_THREADS];
    int args_f[FILTERING_THREADS];

    for (int i = 0; i < FILTERING_THREADS; i++){
        args_f[i] = i * SIZE/FILTERING_THREADS;

        if(pthread_create(&filtering_threads[i], NULL, thread_filtering, (void*)&args_f[i]))
            perror("Error creating thread.");
    }

    //COMPUTING
    pthread_t computing_threads[COMPUTING_THREADS];
    int args_c[COMPUTING_THREADS];

    for (int i = 0; i < COMPUTING_THREADS; i++){
        args_c[i] = i + 1;

        if(pthread_create(&computing_threads[i], NULL, thread_computing, (void*)&args_c[i]))
            perror("Error creating thread.");
    }


    //SIGNALING
    //Wait for all filtering threads to be finished and signal other threads
    for (int i = 0; i < FILTERING_THREADS; i++) {
        if (pthread_join(filtering_threads[i], NULL))
            perror("Failed to join thread");
    }
    printf("Filtering concluded\n");
    sleep(1);
    pthread_cond_broadcast(&cond_filtering);
    //Wait for all computing threads to be finished
    for (int i = 0; i < COMPUTING_THREADS; i++)
        if (pthread_join(computing_threads[i], NULL))
            perror("Failed to join thread");
    printf("Computing concluded\n");


    //Destroy mutex
    pthread_mutex_destroy(&mutex_vec1);
    pthread_mutex_destroy(&mutex_vec2);
    pthread_mutex_destroy(&mutex_vec3);
    pthread_mutex_destroy(&mutex_cost);
    pthread_mutex_destroy(&mutex_computing);
    //Destroy condition variable
    pthread_cond_destroy(&cond_filtering);


    //PRINT RESULT
    printf("Lowest Cost: ");
    printf("Hypermarket %d    Cost = %d\n", lowestCost.id_h, lowestCost.cost);


    return 0;
}