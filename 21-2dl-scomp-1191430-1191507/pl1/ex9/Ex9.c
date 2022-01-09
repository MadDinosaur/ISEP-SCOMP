#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void func_print(int i)
{
    for(int j = (i*100) + 1; j<= (i*100) + 100; j++)
    {
        printf("Number: %d \n", j);
    }
}

int main(void) {

    int status,j=0;

    for(int i=0; i<10; i++) //Processo para criar os 10 processos filho
    {
        if(fork()==0)
        {
            func_print(i);
            printf("Process number: %d \n", i);
            printf("\n");

            exit(status); // Exit status para dar um valor para o processo pai esperar pelos processos filho.
        }
    }

    for (int d=0; d<10; d++)
    {
    	wait(&status);
    }

    printf("Process ended. Shutting down... \n");

    // Não será possível manter a ordem, isto porque devido aos processos serem concorrentes não será possível saber qual o processo que é executado antes ou depois.
}
