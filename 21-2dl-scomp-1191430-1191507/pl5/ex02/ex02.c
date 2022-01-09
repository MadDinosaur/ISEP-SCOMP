#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int number;
    char* name;
    int grade;
} Grade;

void* printer_thread(void* arg) {
    Grade elem = *(Grade*) arg;
    printf("Number: %d, Name: %s, Grade: %d\n", elem.number, elem.name, elem.grade);
    
    //Ends thread
    pthread_exit((void*)NULL);
}

int main(void) {
    //Define array and allocate variables for threads
    Grade array[5];
    int args[5];
    //Define threads
    pthread_t thread_id[5];

    //Initialize array
    Grade elem1;
    elem1.grade = 10;
    elem1.name = "name1";
    elem1.number = 1;
    array[0] = elem1;
    Grade elem2;
    elem2.grade = 20;
    elem2.name = "name2";
    elem2.number = 2;
    array[1] = elem2;
    Grade elem3;
    elem3.grade = 30;
    elem3.name = "name3";
    elem3.number = 3;
    array[2] = elem3;
    Grade elem4;
    elem4.grade = 40;
    elem4.name = "name4";
    elem4.number = 4;
    array[3] = elem4;
    Grade elem5;
    elem5.grade = 50;
    elem5.name = "name5";
    elem5.number = 5;
    array[4] = elem5;


    for(int i = 0; i < 5; i++) {
        //Allocate a separate index for each thread
        args[i] = i;
        //Creates a thread to print array element
        if (pthread_create(&thread_id[i], NULL, &printer_thread, (void*) &array[args[i]]) != 0) {
            perror("Failed to create thread");
        }
    }

    //Waits for all threads to finish execution
    for(int i = 0; i < 5; i++)
        if (pthread_join(thread_id[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    
    return 0;
}