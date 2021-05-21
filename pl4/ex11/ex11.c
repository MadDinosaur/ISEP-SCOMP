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

#define NBABIES 2
#define DOORS "doors"
#define SEMAPHORE_CAPACITY "capacity"
#define SPACE_LEFT "space"
#define TRAIN_CAPACITY 200

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

int main()
{

	sem_t *train_doors, *train_capacity, *train_space;

	train_doors = sem_open(DOORS,O_CREAT | O_EXCL,0644,3);
	train_capacity = sem_open(SEMAPHORE_CAPACITY,O_CREAT | O_EXCL,0644,TRAIN_CAPACITY);
	train_space = sem_open(SPACE_LEFT,O_CREAT | O_EXCL,0644,0);

	if(train_doors == SEM_FAILED || train_capacity == SEM_FAILED || train_space == SEM_FAILED)
	{
		perror("Error processing semaphores");
	}

	int id = babyMaker(NBABIES);

	if(id == -1)
	{
		perror("Error processing children");
	}

	else if(id == 1)
	{
		for(int i = 1; i<= TRAIN_CAPACITY; i++)
		{
			down(train_doors);
			down(train_capacity);
			up(train_space);
			printf("Passenger %d is leaving. \n",i);
			up(train_doors);
		}
		exit(-1);
	}

	else if(id == 2)
	{
		for(int i = 1; i<= TRAIN_CAPACITY; i++)
		{
			down(train_doors);
			up(train_capacity);
			down(train_space);
			printf("Passenger %d is entering. \n",i);
			up(train_doors);
		}
		exit(-1);
	}

	else
	{
		while(wait(NULL) != -1)
		{

		}
		printf("Train is parting.\n");
	}

	printf("Job done. Shutting down...\n");

	sem_unlink(DOORS); sem_unlink(SEMAPHORE_CAPACITY); sem_unlink(SPACE_LEFT);

	return 0;
}
