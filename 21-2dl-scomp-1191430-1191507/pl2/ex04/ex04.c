#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 100
#define FILE_NAME "file.txt"

int main (void) {
    int fd[2];
    pid_t pid;

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
        
        //reads from file
        FILE *fp;
        fp = fopen(FILE_NAME, "r");
        if (fp == NULL) {
            perror("Could not open file");
            return 1;
        }
        while(fgets(str, BUFFER_SIZE, fp) != NULL) {
            //writes to pipe
            write(fd[1], str, BUFFER_SIZE);
        }
        fclose(fp);

        //closes write descriptor
        close(fd[1]);
    } else if (pid == 0) {
        //closes write descriptor - unused by child
        close(fd[1]);
        printf("Text file transcription:\n");
        //reads from pipe
        while (read(fd[0], str, BUFFER_SIZE) > 0)
            printf("%s", str);
        
        //closes read descriptor
        close(fd[0]);
    } else {
        return 1;
    }
return 0;
}