#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define NBABIES 5
#define ARRAY_SIZE 1000
#define INFERIOR_ARRAY_SIZE 200

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

void populateArray(int vec[])
{
	for(int i = 0;i<ARRAY_SIZE;i++)
	{
		vec[i] = rand()%255;
	}
}

int main()
{
	time_t t;
	srand((unsigned)time(&t));

	int fd[5][2];
	int vec1[ARRAY_SIZE];
	int vec2[ARRAY_SIZE];
	int result[INFERIOR_ARRAY_SIZE];
	int final_result[ARRAY_SIZE];

	populateArray(vec1);
	populateArray(vec2);

	int k;
	for (k = 0; k < 5; k++)
	{
		if (pipe(fd[k]) == -1)
		{
			perror("Error processing pipes.\n");
			return -1;
		}
	}

	int id = babyMaker(NBABIES);

	if(id == 0)
	{
		int a;
		babyFuneral(NBABIES);

		for(int j=0; j<5;j++)
		{
			close(fd[j][1]);
			read(fd[j][0],result,sizeof(result)+1);

			for(int i = 0; i<INFERIOR_ARRAY_SIZE;i++)
			{
				a = i + j*(INFERIOR_ARRAY_SIZE);
				final_result[a]=result[i];
			}

			close(fd[j][0]);
		}

		for(int d = 0; d<ARRAY_SIZE;d++)
		{
			printf("FINAL_RESULT: %d\n",final_result[d]);
		}

	}

	if(id > 0)
	{
		close(fd[id-1][0]);

		int b,sum;
		for(int i = 0;i < INFERIOR_ARRAY_SIZE;i++)
		{
			b = i + (id-1)*(INFERIOR_ARRAY_SIZE);
			sum = vec1[b]+vec2[b];
			result[i] = sum;
		}

		write(fd[id-1][1],result,sizeof(result)+1);
		close(fd[id-1][1]);

		exit(0);
	}

	printf("Job done. Shutting down...\n");

	return 0;
}
