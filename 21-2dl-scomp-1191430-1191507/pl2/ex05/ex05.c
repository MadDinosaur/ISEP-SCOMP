#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define NBABIES 1
#define BUFSIZE 256

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

void convertString(char *str)
{
	for(int i = 0; i < strlen(str)-1;i++)
	{
		if(str[i] >= 'A' && str[i] <= 'Z')
		{
			str[i]+=32;
		}
		else if(str[i] >= 'a' && str[i] <= 'z')
		{
			str[i]-=32;
		}
	}
}

int main()
{
	int up[2];
	int down[2];
	char frase[BUFSIZE];
	char frase_new[BUFSIZE];

	if(pipe(up)==-1 || (pipe(down)==-1) )
	{
		perror("Error processing pipe up or down.\n");
		return -1;
	}

	int id = babyMaker(NBABIES);

	if(id==0)
	{
		close(up[1]);

		read(up[0],frase_new,sizeof(frase_new)+1);

		convertString(frase_new);

		close(up[0]);
		close(down[0]);
		write(down[1],frase_new,sizeof(frase_new)+1);
		close(down[1]);

		babyFuneral(NBABIES);
	}

	else if(id > 0)
	{
		close(up[0]);

		printf("Insert your message: \n");
		fgets(frase,BUFSIZE,stdin);

		write(up[1],frase,sizeof(frase)+1);
		close(up[1]);

		close(down[1]);
		read(down[0],frase_new,sizeof(frase_new)+1);
		printf("Client: %s \n",frase_new);
		close(down[0]);
		exit(0);
	}

	else
	{
		printf("ERROR\n");
	}

	printf("Job done. Shutting down.\n");

	return 0;
}
