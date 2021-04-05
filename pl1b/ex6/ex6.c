#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

volatile sig_atomic_t USR1_COUNTER = 1;

void handle_signal(int sig)
{
    sigset_t mask;
    sigfillset(&mask);
    sigprocmask(SIG_BLOCK, &mask, 0); // Serve para bloquear todos os sinais.

    // Para utilizar a funcionalidade pedida no exercício 6 e), servindo para não bloquear nenhum sinal.
    // sigprocmask(SIG_UNBLOCK, &mask, 0);

    printf("SIGUSR1 signal captured, USR1_COUNTER = %d\n", USR1_COUNTER++);
    sleep(1);


}

void handle_signal_more(int sig)
{
    sigset_t mask;
    sigfillset(&mask);
    sigprocmask(SIG_BLOCK, &mask, 0);

    // Para utilizar a funcionalidade pedida no exercício 6 e), servindo para não bloquear nenhum sinal.
    // sigprocmask(SIG_UNBLOCK, &mask, 0);

    printf("SIGUSR1 signal captured, USR1_COUNTER = %d\n", USR1_COUNTER++);
    sleep(3);
}

void pl1b_ex6c()
{
	srand(time(NULL));
	int number = rand() % 16;

    pid_t pid;
    pid = fork();

    if (pid == 0)
    {
        kill(getpid(), 10);

        for (int i = 0; i < 12; i++)
        {
            kill(getpid(), number);
            struct timespec tim, tim2;
            tim.tv_nsec = 10000000;
            tim.tv_sec = 0;
            nanosleep(&tim, &tim2);
        }
    }

    else
    {
        for(;;)
        {
            printf("I'm working!\n");
            sleep(1);
        }
    }
}

int main(int argc, char *argv) {

    struct sigaction act;
    act.sa_handler = &handle_signal;
    // act.sa_handler = &handle_signal_more; // Para executar a funcionalidade no exercício 6 d), aumentando o tempo de sleep para 3 em vez de 1.

    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);

    // pl1b_ex6c(); // Para executar a funcionalidade pedida no exercício 6 c).

	return 0;
}
