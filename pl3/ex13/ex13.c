#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

#define NBABIES 3
#define MINIMUM_SIZE 100
#define MAXIMUM_SIZE 1000

typedef struct
{
    char paths[NBABIES][MINIMUM_SIZE];
    char word_search[NBABIES][MINIMUM_SIZE];
    int occurences[NBABIES];
}Struct;

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

int search_file(char *path, char *word)
{
	int count = 0;

	FILE *file;
	file = fopen(path,"r");

	if(file == NULL)
	{
		perror("Can't open the file.");

		return -1;
	}

	char string[MAXIMUM_SIZE];
	while(fgets(string,MAXIMUM_SIZE,file)!= NULL)
	{
		int size_word = strlen(word);
		int size_string = strlen(string);
		int temp, len = 0;

		for(int i = 0; i < size_string; i++)
		{
			temp = i;

			for(int j = 0; j < size_word; j++)
			{
				if(string[i] == word[j])
				{
					i++;
				}
			}

			len = i - temp;

			if(len == size_word)
			{
				count++;
			}

			len = 0;
			i = temp;
		}
	}

	fclose(file);

	return count;
}

Struct initialize()
{
	char word[MINIMUM_SIZE];
	Struct obj;

	for(int i = 0; i<NBABIES; i++)
	{
		strcpy(obj.paths[i], "file.txt");
		printf("Choose the word to be searched in the file: ");
		scanf("%s",word);
		strcpy(obj.word_search[i],word);
		obj.occurences[i] = 0;
	}

	return obj;
}

int main() {

	Struct *shared_memory_data;

	int fd, data_size = sizeof(Struct);

	fd = shm_open("/shmtest",O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);

	ftruncate(fd,data_size);

	shared_memory_data = (Struct *)mmap(NULL,data_size,PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if (shared_memory_data == NULL)
	{
		perror("No mmap()");
		exit(-1);
	}

	*shared_memory_data = initialize();

	int id = babyMaker(NBABIES);

	if(id == -1)
	{
		perror("Error processing child.\n");
	}
	else if(id == 0)
	{
		int status;
		while(wait(&status) != -1)
		{
			int i = WEXITSTATUS(status) - 1;
			if(shared_memory_data->occurences[i] != -1)
			{
				printf("NUMBER OF OCCURENCES OF THE WORD %s: %d \n",shared_memory_data->word_search[i],shared_memory_data->occurences[i]);
			}
		}
	}
	else
	{
		shared_memory_data->occurences[id-1] = search_file(shared_memory_data->paths[id - 1], shared_memory_data->word_search[id-1]);
		exit(id);
	}

	if(munmap(shared_memory_data, data_size)<0)
	{
		perror("No munmap()");
		exit(-1);
	}

	if(close(fd)<0)
	{
		perror("No close()");
		exit(-1);
	}

	if(shm_unlink("/shmtest")<0){
		perror("No unlink()");
		exit(1);
	}

	printf("Job done. Shutting down... \n");

	return 0;
}
