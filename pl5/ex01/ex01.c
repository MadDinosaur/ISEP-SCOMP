#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* thread_func(){
    printf("Thread was created\n", pthread_self());
    pthread_exit((void*)NULL);
}

int main() {
    pid_t pid = fork();
    printf("Process running after fork no. 1\n");

    if(pid == 0) {
    fork();
    printf("Process runnning after fork no. 2\n");
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, &thread_func, NULL);
    pthread_join(thread_id, NULL);
    }

    fork();
    printf("Process running after fork no. 3\n");
}

//How many processes are created? 
//6 processes 
// |
// |-----------| Fork no. 1
// |           |
// |-----|     | Fork no. 2 (only happens on parent)
// |     |     |
// |---| |---| |---| Fork no. 3
// |   | |   | |   |

//How many threads are created?
//2 threads
//One for each process running inside if statement after Fork no. 2
