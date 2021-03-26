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
		array[i] = rand() % 256;	
}

int verificar_max(int* array,int j)
{	
	// recebe um apontador para a primeira posição do array e verifica o maior entre cada 200 valores.
	int max = 0;
	
	for(int p = j*200; p < (j+1)*200; p++)
	{
		if(array[p]> max)
		max = array[p];
	}	
	return max;
}

void calculo(int* array,int* array2,int maximum)
{
	pid_t p = fork();

		if(p == 0)
		{
			printf("VEC FILHO: ");
			for(int f = 0; f<ARRAY_SIZE/2;f++)
			{
				array2[f] = ((int)array[f]/maximum)*100;
				printf(" %d |",array2[f]);
			}
			printf("\n");
			exit(0);
		}

		else if(p == -1)
		{
			printf("ERROR");
		}

		else
		{
			printf("VEC PAI: ");
			for(int w = ARRAY_SIZE/2; w<ARRAY_SIZE;w++)
			{
			   array2[w] = ((int)array[w]/maximum)*100;
			   printf("VEC PAI: %d \n", array2[w]);
			}
			
			printf("\n");
			wait(NULL);
		}			
}

int main(void) {

	// permitir o randomizer funcionar devidamente
	time_t t;
	srand((unsigned) time (&t));

	int array[ARRAY_SIZE];
	
	popular_array(array); // popula o array
	
	//inicializar o array principal, para o processo filho e processo pai fazerem as suas metades.
	int result[ARRAY_SIZE];
	
	int maximum = 0, status = 0;

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
		wait(&status);
		
		if(WEXITSTATUS(status) > maximum)
		maximum = WEXITSTATUS(status);
	}

	calculo(array,result,maximum);

	printf("Process ended. Shutting down...");
}
