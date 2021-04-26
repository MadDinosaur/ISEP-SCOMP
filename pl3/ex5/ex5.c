#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <limits.h>

#define NBABIES 1
#define EXECUTION_TIMES 1000000

int babyMaker(int n)
{
	int p, i;
	for(i=0; i<n; i++)
	{
		p = fork();

		if(p<0)
		{
			exit(-1); //ERRO
		}

		if(p==0)
		{
			return i+1; //FILHO i+1
		}
	}
	return 0; //PAI
}

void babyFuneral(int n)
{
	int i, status, exitcode, p;
	for(i=0; i<n; i++)
	{
		p = waitpid(-1,&status,0);

		if(WIFEXITED(status))
		{
			exitcode = WEXITSTATUS(status);
			printf("Baby %d has gone with exit code %d\n", p, exitcode);
		}
	}
}

int main() {

	int *map;

	int fd, size=sizeof(map);

	if((fd = shm_open("/shmtest", O_CREAT | O_RDWR | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO)) <0)
		perror("Failed to create shared memory");

	if(ftruncate(fd, size)<0)
		perror("Failed to adjust memory size");

	map = (int *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if (map == NULL) {
		perror("No mmap()");
		exit(0);
	}

	int value1 = 8000;
	int value2 = 200;

	int id = babyMaker(NBABIES);

	if(id == -1)
	{
		printf("Error processing child.\n");
	}

	else if(id == 0)
	{
		babyFuneral(NBABIES);

		for(int i = 0; i < EXECUTION_TIMES;i++)
		{
			value1++;
			value2--;
		}
	}

	else
	{
		for(int i = 0; i < EXECUTION_TIMES;i++)
		{
			value1--;
			value2++;
		}

		exit(0);
	}

	if(munmap(map, size)<0){
		perror("No munmap()");
		exit(0);
	}

	if(close(fd)<0){
		perror("No close()");
		exit(0);
	}

	printf("Value nº 1: %d\n",value1);
	printf("Value nº 2: %d\n",value2);

	printf("Job done. Shutting down...\n");

	return 0;
}
