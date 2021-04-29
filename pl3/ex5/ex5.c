#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NBABIES 1
#define EXECUTION_TIMES 1000000

typedef struct
{
	int value1;
	int value2;
	int number_operations;
}Numbers;

Numbers fill_struct()
{
	Numbers shared_data;
	shared_data.value1 = 8000;
	shared_data.value2 = 200;
	shared_data.number_operations = 0;

	return shared_data;
}

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

	Numbers *shared_memory_data;

	int fd, size=sizeof(shared_memory_data);

	if((fd = shm_open("/shmtest", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR)) <0)
		perror("Failed to create shared memory");

	if(ftruncate(fd, size)<0)
		perror("Failed to adjust memory size");

	shared_memory_data = (Numbers *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if (shared_memory_data == NULL) {
		perror("No mmap()");
		exit(0);
	}

	*shared_memory_data = fill_struct();

	int id = babyMaker(NBABIES);

	if(id == -1)
	{
		printf("Error processing child.\n");
	}

	else if(id == 0)
	{
		while(shared_memory_data->number_operations < 2 * EXECUTION_TIMES)
		{
			if(shared_memory_data->number_operations % 2 == 0)
			{
				shared_memory_data->value1++;
				shared_memory_data->value2--;
				shared_memory_data->number_operations++;
			}
		}
	}

	else
	{
		while(shared_memory_data->number_operations < 2 * EXECUTION_TIMES)
		{
			if(shared_memory_data->number_operations % 2 == 1)
			{
				shared_memory_data->value1--;
				shared_memory_data->value2++;
				shared_memory_data->number_operations++;
			}
		}

		exit(0);
	}

	printf("Value nº 1: %d\n",shared_memory_data->value1);
	printf("Value nº 2: %d\n",shared_memory_data->value2);
	printf("Number of operations: %d\n",shared_memory_data->number_operations);

	if(munmap(shared_memory_data, size)<0){
		perror("No munmap()");
		exit(0);
	}

	if(close(fd)<0){
		perror("No close()");
		exit(0);
	}

    if(shm_unlink("/shmtest")<0){
        perror("No unlink()");
        exit(1);
    }

	printf("Job done. Shutting down...\n");

	return 0;
}
