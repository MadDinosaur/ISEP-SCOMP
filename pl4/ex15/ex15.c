#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int babyMaker(int n)
{
	int p, i;
	for(i=0; i<n; i++){
		p = fork();
		if(p<0) exit(-1); //ERRO
		if(p==0) return i+1; //FILHO i+1
	}
	return 0; //PAI
}

void down(sem_t *sem) {
    if (sem_wait(sem) == -1) {
        perror("Error at sem_wait().");
        exit(0);
    }
}

void up(sem_t *sem) {
    if (sem_post(sem) == -1) {
        perror("Error at sem_post().");
        exit(0);
    }
}

void printMessage(char *message) {
    printf("%s", message);
    fflush(stdout);
}

void enter(sem_t* sem){
    down(sem);
    printf("I have entered the show\n");
}

void leave (sem_t* sem){
    up(sem);
    printf("I have left the show\n");
}

int main()
{
	sem_t *semaphore;

	if (sem_unlink("/shmtest") == -1)
	{
		perror("Error at sem_unlink().\n");
		printf("Error: %s\n", strerror(errno));
		exit(0);
	}

	if ((semaphore = sem_open("/shmtest", O_CREAT | O_EXCL, 0644, 5)) == SEM_FAILED) {
		perror("Error at sem_open().\n");
		exit(0);
	}

	while(1)
	{
		if (fork()==0)
		{
			enter(semaphore);
			sleep(10); //Simula o tempo que eles passam dentro da sala
			leave(semaphore);
		}
	}

	printf("Job done. Shutting down...\n");

	return 0;
}
