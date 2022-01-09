#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_STRING_SIZE 20

int main (void) {
    pid_t pid;
    char parent_msg[3][20] = {"I'm..\n", "the..\n", "father!\n"};

    int i;
    for(i = 0; i < 3; i++) {
        pid = fork();
        if(pid == 0) {
            printf("I'll never join you!\n");
            exit(0);
        }
        else {
            printf("%s", parent_msg[i]);
        }
    }
    exit(0);
}