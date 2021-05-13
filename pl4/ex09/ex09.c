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
#define SEMAFOROS 2

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

void printMessage(char *message)
{
    printf("%s \n", message);
    fflush(stdout);
}

void buy_beer()
{
	printMessage("Bought beer");
}

void buy_chips()
{
	printMessage("Bought drink");
}

void eat_and_drink()
{
	printMessage("Eaten and drunk");
}


int main()
{
	sem_t *sem[SEMAFOROS];
	for(int i = 0; i<SEMAFOROS; i++)
	{
		char name[20];
		sprintf(name,"%s%d","SEMAFORON",i);

		if ((sem[i] = sem_open(name, O_CREAT | O_EXCL, 0644,0)) == SEM_FAILED)
		{
			perror("Error at sem_open().\n");
			exit(0);
		}
	}

	int id = babyMaker(NBABIES);

	if(id == -1)
	{
		perror("Error processing child.\n");
	}

	else if(id == 0)
	{
		buy_beer();
		up(sem[1]);
		down(sem[0]);
		eat_and_drink();
	}

	else
	{
		down(sem[1]);
		buy_chips();
		up(sem[0]);
		eat_and_drink();
		exit(0);
	}

	wait(NULL);

	for (int i=0;i<NBABIES+1;i++)
	{
		char name[20];
		sprintf(name,"%s%d","SEMAFORON",i);

		if (sem_unlink(name) == -1)
		{
			perror("Error at sem_unlink().\n");
			printf("Error: %s\n", strerror(errno));
			exit(0);
		}
	}

	return 0;
}
