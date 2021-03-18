#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main(void) {

	pid_t parent;
	int n = 100;
	int status,j=1;

	for(int i=0; i<10; i++) //Processo para criar os 10 processos filho
	{
		if(fork()==0)
		{
			for(j; j<=n;j++) // For para printar os 100 números de cada vez por filho.
			{
				printf("Number: %d \n",j);
			}

			exit(status); // Exit status para dar um valor para o processo pai esperar pelos processos filho.
		}

		j+=100;
		n+=100;
	}

	for(int d=0; d<10;d++)
	{
		waitpid(-1,&status,0);
	}

	// Não será possível manter a ordem, isto porque devido ao escalonamento não será possível saber qual o processo filho que está a ocorrer.
	return 0;
}
