#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include<sys/wait.h>

#define STR_SIZE 50
#define NR_DISC 10
struct aluno{
    int numero;
    char nome[STR_SIZE];
    int disciplinas[NR_DISC];
};

int main(void) {
    pid_t pid;
    int fd, data_size = sizeof(struct aluno);
    struct aluno* s;

    //Creates shared memory
    if ((fd = shm_open("/ex12", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR)) < 0)
        perror("Failed to create shared memory");
    //Defines memory size
    if (ftruncate (fd, data_size) < 0)
        perror("Failed to adjust memory size");
    //Maps the area in an address
    s = (struct aluno *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (s == NULL) {
        perror("Failed to map the area");
        exit(1);
    }
    
    //Requests user information
    printf("Insert number:\n");
    scanf("%d", &(s->numero));
    printf("Insert name:\n");
    scanf("%s", &(s->nome));
    printf("Insert grades (separated by Enter):");
    for(int i = 0; i < NR_DISC; i++) {
        scanf("%d", &(s->disciplinas[i]));
    }

    //CHILD
    if ((pid = fork()) > 0) {
        //Accesses shared memory area
        if ((fd = shm_open("/ex12", O_RDWR, S_IRUSR|S_IWUSR)) < 0)
            perror("Failed to create shared memory");
        //Defines memory size
        if (ftruncate (fd, data_size) < 0)
            perror("Failed to adjust memory size");
        //Maps the area in an address
        s = (struct aluno*) mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
        if (s == NULL) {
            perror("Failed to map the area");
            exit(0);
        }

        //Calculates highest, lowest and average grade
        int min = -1, max = -1, avg = 0;
        for (int j = 0; j < NR_DISC; j++) {
            if (min == -1 || min > s->disciplinas[j])
                min = s->disciplinas[j];
            if (max == -1 || max < s->disciplinas[j])
                max = s->disciplinas[j];
            avg = avg + s->disciplinas[j];
        }
        avg = avg/NR_DISC;
        printf("Highest = %d, Lowest = %d, Average = %d\n", max, min, avg);
    } else {
        wait(NULL);
        if(munmap(s, data_size)<0){
            perror("No munmap()");
            exit(1);
        }

        if(close(fd)<0){
            perror("No close()");
            exit(1);
        }

        if(shm_unlink("/ex12")<0){
            perror("No unlink()");
            exit(1);
        }
    }
}