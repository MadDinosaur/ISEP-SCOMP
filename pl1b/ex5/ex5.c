#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

void pl1b_ex5a()
{
	for(;;)
	{
		printf("I Like Signal\n");
		sleep(1);
	}
}

void sig_handler(int sig)
{
    if (sig == 2)
    	printf("\nI won’t let the process end with CTRL-C!\n");

    else if (sig == 3)
    	printf("\n“I won’t let the process end by pressing CTRL-\!\n");
}

void pl1b_ex5b()
{
    signal(SIGINT, sig_handler);

    for (;;)
    {
        printf("I Like Signal B\n");
        sleep(1);
    }
}

void pl1b_ex5d()
{
	signal(SIGINT, sig_handler);

	for (;;)
    {
		printf("I Like Signal C\n");
	    sleep(1);
    }
}

int main() {

	/* Ex5:
	 *
	 * a)
	 *
	 * Após pressionar CTRL-C, foi executado o signal SIGINT que interrompe o programa a ser executado.
	 *
	 * c)
	 *
	 * Após pressionar CTRL-\, foi executado o signal SIGQUIT que dá quit do programa a ser executado.
	 *
	 * e)
	 *
	 * Dependendo do que for usado, SIGINT ou SIGQUIT, a mensagem seria de interruptção ou de quit respetivamente.
	 *
	 * f)
	 *
	 * Após o stop do programa ao pressionar o CTRL-Z, ao executar o comando "kill %1", é morto o primeiro "job".
	 *
	 * g)
	 *
	 * When performing buffered I/O on a file, the stdio functions must maintain a statically allocated data buffer
	 * along with associated counters and indexes (or pointers) that record the amount of data and the current position
	 * in the buffer. Suppose that the main program is in the middle of a call to a stdio function such as printf(3)
	 * where the buffer and associated variables have been partially updated. If, at that moment, the program is interrupted
	 * by a signal handler that also calls printf(3), then the second call to printf(3) will operate on inconsistent data,
	 * with unpredictable results.
	 *
	 */

	// pl1b_ex5a(); // Para executar a funcionalidade pedida no exercício 5 a).
	// pl1b_ex5b(); // Para executar a funcionalidade pedida no exercício 5 b).
	// pl1b_ex5c(); // Para executar a funcionalidade pedida no exercício 5 d).

	return 0;
}
