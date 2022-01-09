#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define NBABIES 1

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

	int fd[2];
	char str[50] = "Hello World";
	char str2[50] = "Goodbye";

	if(pipe(fd)==-1)
	{
		perror("Error producing pipe.\n");
	}

	int id = babyMaker(NBABIES);

	if(id == 0)
	{
		close(fd[0]);
		write(fd[1],str,strlen(str)+1);
		write(fd[1],str2,strlen(str2)+1);
		close(fd[1]);
		babyFuneral(NBABIES);
	}

	else if(id > 0)
	{
		char strNew[50];
		char str2New[50];

		close(fd[1]);
		read(fd[0],strNew,strlen(str)+1);
		read(fd[0],str2New,strlen(str2)+1);
		close(fd[0]);

		printf("Child reading: %s\n",strNew);
		printf("Child reading: %s\n",str2New);

		exit(0);
	}

	else
	{
		printf("Internal Error.\n");
	}

	printf("Job done. Shutting down.\n");

	return 0;
}
