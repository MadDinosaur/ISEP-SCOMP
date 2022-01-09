#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <limits.h>

#define NBABIES 10
#define ARRAY_SIZE 50000
#define INFERIOR_ARRAY_SIZE 5000
#define PERCENTAGE 40

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

typedef struct
{
  int customer_code;
  int product_code;
  int quantity;
}product;

void populateArray(product array_products[])
{
	for (int i = 0; i < 50000; i++)
	{
		product temp = {temp.customer_code = rand() % INT_MAX, temp.product_code = rand() % INT_MAX, temp.quantity = rand() % PERCENTAGE};
		array_products[i] = temp;
	}
}

int main() {

	time_t t;
	srand((unsigned)time(&t));

	int fd[10][2];
	product array[ARRAY_SIZE];
	int result[ARRAY_SIZE];
	int intermediate_result[INFERIOR_ARRAY_SIZE];

	populateArray(array);

	int i;
	for (i = 0; i < 5; i++)
	{
		if (pipe(fd[i]) == -1)
		{
			perror("Error processing pipes.\n");
			return -1;
		}
	}

	int id = babyMaker(NBABIES);

	if(id == 0)
	{
		babyFuneral(NBABIES);

		int a,count=0;

		for(int j=0;j<10;j++)
		{
			close(fd[j][1]);
			read(fd[j][0],intermediate_result,sizeof(intermediate_result)+1);

			for(int l=0;l<INFERIOR_ARRAY_SIZE;l++)
			{
				a = l + j*(INFERIOR_ARRAY_SIZE);
				result[a]=intermediate_result[l];
			}

			close(fd[j][0]);
		}

		for(int d=0;d<ARRAY_SIZE;d++)
		{
			if(result[d]>0)
			{
				printf("CODE: %d\n",result[d]);
				count++;
			}
		}

		printf("TOTAL SALES: %d\n",count);
	}

	else if(id > 0)
	{
		close(fd[id-1][0]);

		int b;
		for(int i = 0;i < INFERIOR_ARRAY_SIZE;i++)
		{
			b = i + (id-1)*(INFERIOR_ARRAY_SIZE);
			if(array[b].quantity > 20)
			{
				intermediate_result[i]=array[b].product_code;
			}
		}

		write(fd[id-1][1],intermediate_result,sizeof(intermediate_result)+1);
		close(fd[id-1][1]);

		exit(0);
	}

	else
	{
		printf("Error processing children.\n");
	}

	printf("Job done. Shutting down...\n");

	return 0;
}
