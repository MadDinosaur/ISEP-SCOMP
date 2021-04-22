#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#define NBABIES 1
#define NAME_SIZE 50

typedef struct{
	char name[NAME_SIZE];
	int number;
}Student;

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


int main()
{

	Student *s;

	int fd;
	int size = sizeof(Student);

	if((fd = shm_open("/shmtest", O_CREAT | O_RDWR | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO)) <0)
		perror("Failed to create shared memory");

	if(ftruncate(fd, size)<0)
		perror("Failed to adjust memory size");

	s = (Student *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (s == NULL) {
        perror("No mmap()");
        exit(0);
    }

	int id = babyMaker(NBABIES);

	if(id == -1)
	{
		printf("Error processing child\n");
	}

	else if(id == 0)
	{
		babyFuneral(NBABIES);

		printf("\nName = %s\n", s->name);
		printf("Number = %d\n", s->number);

		if(munmap(s, size)<0){
			perror("No munmap()");
			exit(0);
		}

		if(close(fd)<0){
			perror("No close()");
			exit(0);
		}

		if(shm_unlink("/shmtest")<0){
			perror("No unlink()");
			exit(1);
		}
	}

	else
	{
		if (s == NULL) {
			perror("No mmap()");
			exit(0);
		}

		printf("\nInsert the student's name: ");
		fgets(s->name, sizeof(s->name), stdin);

		fflush(stdin);

		printf("\nInsert the student's number: ");
		scanf("%d", &(s->number));
		printf("\n");

		if(munmap(s, size) < 0){
			perror("No munmap()");
			exit(0);
		}

		if(close(fd) < 0){
			perror("No close()");
			exit(0);
		}

		exit(0);
	}

	printf("\nJob done. Shutting down...\n");

	return 0;
}
