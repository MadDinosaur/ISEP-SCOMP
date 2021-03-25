#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define ARRAY_SIZE 1000


void popularArray(int* array)
{
	time_t t;
	srand((unsigned) time (&t));

	for(int i = 0; i<ARRAY_SIZE;i++)
	{
		array[i] = rand() % 255;
	}

}

int verificarMax(int* array)
{
	for(int j = 0; j<5; j++)
	{
		int max = 0;

		if(fork() == 0)
		{
			for(int p = (j*200); p<(j*200)+200; p++)
			{
				if(array[p]> max)
				{
					max = array[p];
				}
			}

			exit(max);
		}
	}
}

int calculo(int* array,int* array2,int maximum)
{
	pid_t parent = fork();

		if(parent == 0)
		{
			printf("TESTE TESTE CALCULO FILHO: %d,%d,%d \n", array[2],array2[2],maximum);

			for(int f = 0; f<ARRAY_SIZE/2;f++)
			{
				array2[f] = ((int)array[f]/maximum)* 100;
				printf("Output: %d \n",array2[f]);
			}

			exit(0);
		}

		if(parent == -1)
		{
			printf("ERROR");
		}

		else
		{
			printf("TESTE TESTE CALCULO PAI: %d,%d,%d \n", array[2],array2[2],maximum);

			for(int w = (ARRAY_SIZE/2); w <ARRAY_SIZE;w++)
			{
			   array2[w] = ((int)array[w]/maximum) * 100;
			   printf("%.2f,%d,%d \n",(array[w]/maximum),array[w],maximum);
			   printf("Output: %d \n", array2[w]);
			}

			wait(NULL);
		}

}

int main(void) {

	int array[ARRAY_SIZE];

	popularArray(array); // popula o array

	printf("Hello: %d \n",array[2]);

	//inicializar os arrays com metade do valor do array principal, para o processo filho e processo pai fazerem as suas metades.
	int result[ARRAY_SIZE];

	int status;
	int maximum = 0;

	// criar os processos filhos para determinarem qual é o valor max em 1/5 do array total.
	verificarMax(array);

	// espera pelos processos filhos todos terminarem para determinar o valor max de todo o array.
	for(int d = 0; d<5;d++)
	{
		wait(&status);
		if(WEXITSTATUS(status) > maximum)
		maximum = WEXITSTATUS(status);
		printf("Max[%d] = %d \n", d,maximum);
	}

	printf("TESTE TESTE %d,%d,%d \n", array[2],result[2],maximum);

	calculo(array,result,maximum);

	printf("Process ended. Shutting down...");
}
