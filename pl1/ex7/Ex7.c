#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define ARRAY_SIZE 1000

int main (void)
{
 	int numbers[ARRAY_SIZE]; /* array to lookup */
 	int n; /* the number to find */
 	time_t t;/* needed to init. the random number generator (RNG)*/
 	int i=0,j=0,p=0,total_segunda=0,total_primeira=0,total_final=0,status=0;
	pid_t parent;

 	/* intializes RNG (srand():stdlib.h; time(): time.h) */
 	srand ((unsigned) time (&t));

 	 /* initialize array with random numbers (rand(): stdlib.h) */
 	for (i = 0; i < ARRAY_SIZE; i++)
	numbers[i] = rand () % 10000;

	/* initialize n */
 	n = rand () % 10000;

	parent = fork();

	if(parent == 0) // Inicia o processo filho e vai procurar pela segunda metade do array
	{
		for(j = (ARRAY_SIZE/2); j <= ARRAY_SIZE;j++)
		{
			if(numbers[j] == n) // Se o número do array for igual ao n, aumenta total para a segunda metade
			{
				total_segunda++;
			}
		}

		exit(total_segunda); // Retorna o valor para ser utilizado no total
	}

	if(parent > 0)
	{
		wait(&status); // Espera pelo retorno do filho.

		for(p = 0; p < (ARRAY_SIZE/2); p++)
		{
			if(numbers[p] == n) // Caso o numero no array seja igual ao numero n
			{
				total_primeira++; // Adiciona o numero de vezes que é encontrada
			}
		}

		total_final = total_primeira + WEXITSTATUS(status); // utilizo WEXITSTATUS porque é confirmado que só aparecerá no máximo 255 vezes.
	}

	if(parent < 0) // o filho não é criado
	{
		printf("Error! Child is not created");
	}

	printf("Number of total times n is in the array: %d \n",total_final);
	printf("The program ends here.");

	return 0;
}
