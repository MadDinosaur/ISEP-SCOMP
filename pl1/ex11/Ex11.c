#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define ARRAY_SIZE 1000


void popular_array(int* array)
{
	// serve apenas para popular o array de 1000 posições com números entre [0-255]. 
	
	for(int i = 0; i<ARRAY_SIZE;i++)
	{
		array[i] = rand() % 256;
	}
}

int verificar_max(int* array,int j)
{	
	// recebe um apontador para a primeira posição do array e verifica o maior entre cada 200 valores.
	
	int max = 0;
	
	for(int p = j*200; p < (j+1)*200; p++)
	{
		if(array[p]> max)
		{
			max = array[p];
		}
	}
	return max;
}

int calculo(int* array,int* array2,int maximum)
{
	pid_t parent = fork();

		if(parent == 0)
		{
			for(int f = 0; f<500;f++)
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
			for(int w = 500; w <ARRAY_SIZE;w++)
			{
			   array2[w] = ((int)array[w]/maximum) * 100;
			   printf("Output: %d \n", array2[w]);
			}
		}	
		
		wait(NULL);
}

int main(void) {

	time_t t;
	srand((unsigned) time (&t));

	int array[ARRAY_SIZE];
	
	popular_array(array); // popula o array
	
	//inicializar o array principal, para o processo filho e processo pai fazerem as suas metades.
	int result[ARRAY_SIZE];
	
	int maximum = 0;

	// criar os processos filhos para determinarem qual é o valor max em 1/5 do array total.
	for(int j=0;j<5;j++)
	{
		if(fork()==0)
		{
			exit(verificar_max(array,j));
		}
	}
	
	// espera pelos processos filhos todos terminarem para determinar o valor max de todo o array.
	for(int d = 0; d<5;d++)
	{
		int status = 0;
		wait(&status);
		
		if(WEXITSTATUS(status) > maximum)
		maximum = WEXITSTATUS(status);
	}

	calculo(array,result,maximum);

	printf("Process ended. Shutting down...");
}
