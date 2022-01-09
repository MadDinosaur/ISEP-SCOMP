#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 100

int main (void) {
    int fd[2];
    pid_t pid;

    int i;
    char str[BUFFER_SIZE];

    //parent creates pipe
    if(pipe(fd) == -1) {
        perror("Pipe failed");
        return 1;
    }

    pid = fork();
    if (pid > 0) {
        //closes read descriptor - unused by parent
        close(fd[0]);
        
        //reads from stdin
        printf("Insert an integer: ");
        scanf("%d", &i);

        printf("Insert a string: ");
        scanf("%s", str);

        printf("===================\n");
 
        //writes to pipe
        write(fd[1], &i, BUFFER_SIZE);
        write(fd[1], str, BUFFER_SIZE);

        //closes write descriptor
        close(fd[1]);
    } else if (pid == 0) {
        //closes write descriptor - unused by child
        close(fd[1]);
        
        //reads from pipe
        read(fd[0], &i, BUFFER_SIZE);
        read(fd[0], str, BUFFER_SIZE);
        printf("Inserted integer is: %d.\n", i);
        printf("Inserted string is: %s.\n", str);
        
        //closes read descriptor
        close(fd[0]);
    } else {
        return 1;
    }
return 0;
}