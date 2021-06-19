#include<stdio.h>
#include<pthread.h>

#define THREADS 4

typedef struct {
    int limit;
    int interval;
} Primes;
Primes primes;

void ask_for_number() {
    printf("Please insert an unsigned integer:\n");
    scanf("%d", &primes.limit);
}

void *print_prime(void *arg)
{
    int isPrime, end;
    int curr_number = *(int*) arg;
    if((end = curr_number + primes.interval) > primes.limit)
        end = primes.limit;
    printf("Analysing prime numbers between %d and %d\n", curr_number, end);
    
    while(curr_number < end) {
        isPrime = 1;
        for(int i = 2; i <= curr_number/2; i++) {
            if(curr_number % i == 0) {
                isPrime = 0;
                break;
            }
        }

        if(isPrime && (curr_number > 1))
        {
           printf("%d\n", curr_number);
        }
        curr_number++;
  }
}

int main() {
    //Ask the user for an unsigned int
    ask_for_number();

    //CREATE THREADS
    pthread_t threads[THREADS];
    int args[THREADS];
    primes.interval = primes.limit/(THREADS-1);
    printf("Calculating prime numbers with %d threads in intervals of %d\n", THREADS, primes.interval);
    
    for(int i = 0; i < THREADS; i++) {
        if(i == THREADS - 1)
            args[i] = (THREADS - 1)*primes.interval;
        else    
            args[i] = i * primes.interval;
        if(pthread_create(&threads[i], NULL, print_prime, (void*)&args[i]))
            perror("Error creating thread.");
    }
    
    //WAIT FOR THREADS TO FINALIZE
    for(int i = 0; i < THREADS; i++)
        if (pthread_join(threads[i], NULL))
            perror("Failed to join thread");

    return 0;
 }