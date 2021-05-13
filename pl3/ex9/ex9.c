#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define NBABIES 10
#define FULL_SIZE 1000
#define INTERMEDIATE_SIZE 100
#define SHORTER_SIZE 10
#define MAX_VALUE 0

typedef struct
{
	int array_ten_elements[SHORTER_SIZE];
}Estrutura;

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

void fill_array(int array_with_all_values[])
{
	time_t t;
	srand((unsigned)time(&t));

	for(int i=0; i<FULL_SIZE;i++)
	{
		array_with_all_values[i] = rand() % 2500;
	}
}

int main() {

	Estrutura *shared_memory_data;

	int fd, size=sizeof(shared_memory_data);

	if((fd = shm_open("/shmtest", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR)) <0)
		perror("Failed to create shared memory");

	if(ftruncate(fd, size)<0)
		perror("Failed to adjust memory size");

	shared_memory_data = (Estrutura *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if (shared_memory_data == NULL) {
		perror("No mmap()");
		exit(0);
	}

	//VARIAVIES USADAS PELO PAI
	int global_max_value = 0;
	int global_new_max_value = 0;

	//VARIAVEIS USADAS PELO FILHO
	int max_value = 0;
	int new_max_value = 0;

	int array_with_all_values[FULL_SIZE];

	fill_array(array_with_all_values);

	int id = babyMaker(NBABIES);

	if(id == -1)
	{
		printf("Error processing child.\n");
	}

	else if(id == 0)
	{
		babyFuneral(NBABIES);

		for(int i = 0; i < NBABIES;i++)
		{
			if(shared_memory_data->array_ten_elements[i]>MAX_VALUE)
			{
				global_max_value = shared_memory_data->array_ten_elements[i];

				if (global_max_value > global_new_max_value)
				{
					global_new_max_value = global_max_value;
				}
			}

			printf("PAI: %d \n", shared_memory_data->array_ten_elements[i]);
		}
	}

	else
	{
		for(int j = 0; j<NBABIES; j++)
		{
			for(int i=(j*INTERMEDIATE_SIZE); i < (j*INTERMEDIATE_SIZE)+(INTERMEDIATE_SIZE);i++)
			{
				if(array_with_all_values[i] > MAX_VALUE)
				{
					 max_value = array_with_all_values[i];

						if(max_value > new_max_value)
						{
							new_max_value = max_value;
						}
				}

			}

			/* printf("FILHO: %d\n", new_max_value); */

			shared_memory_data->array_ten_elements[j] = new_max_value;
		}

		exit(0);
	}

	printf("GLOBAL MAX VALUE: %d\n",global_new_max_value);

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
