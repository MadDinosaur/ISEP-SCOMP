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

#define NBABIES 1
#define N_REPETIONS 15

int babyMaker(int n)
{
	int p, i;
	for(i=0; i<n; i++)
	{
		p = fork();
		if(p<0) exit(-1); //ERRO
		if(p==0) return i+1; //FILHO i+1
	}
	return 0; //PAI
}

void down(sem_t *sem)
{
    if (sem_wait(sem) == -1)
    {
        perror("Error at sem_wait().");
        exit(0);
    }
}

void up(sem_t *sem)
{
    if (sem_post(sem) == -1)
    {
        perror("Error at sem_post().");
        exit(0);
    }
}

int main() {

	sem_t *sempai;
	sem_t *semfilho;

	if ((sempai = sem_open("semaforo", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
	{
		perror("Error at sem_open().\n");
		exit(0);
	}

	if((semfilho = sem_open("semaforo2",O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
	{
		perror("Error at sem_open().\n");
		exit(0);
	}

	int id = babyMaker(NBABIES);

	if(id == -1)
	{
		perror("Error processing child.\n");
	}

	else if(id == 0)
	{
		for(int i = 0; i<N_REPETIONS/2; i++)
		{
			down(sempai);
			printf("I'm the father.\n");
			up(semfilho);
		}
	}

	else
	{
		for(int i = 0; i<=N_REPETIONS/2;i++)
		{
			down(semfilho);
			printf("I'm the child.\n");
			up(sempai);
		}
	}

	if (sem_close(sempai) == -1 && sem_close(semfilho) == -1)
	{
		perror("Error at sem_close().\n");
		exit(0);
	}

	if (sem_unlink("semaforo") == -1 && sem_unlink("semaforo2") == -1)
	{
		perror("Error at sem_unlink().\n");
		printf("Error: %s\n", strerror(errno));
		exit(0);
	}

	return 0;
}
