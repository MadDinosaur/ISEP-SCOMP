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
		}

		exit(0);
	}

	printf("GLOBAL MAX VALUE: %d\n",global_new_max_value);

	printf("Job done. Shutting down...\n");

	return 0;
}
