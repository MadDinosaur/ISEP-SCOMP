#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>


void pl1b_ex2()
{
	for(;;)
	{
		printf("I Love SCOMP!\n");
		sleep(1);
	}
}

void pl1b_ex2e()
{
    int pid;
    scanf("%d", &pid);
    int signal = 0;
    scanf("%d", &signal);

	if (!kill(pid, signal))
		printf("DONE\n");
	else
		printf("NOT DONE \n");
}


int main() {

	/* EX2:
	 *
	 * a)
	 *
	 * Step 1: gcc -Wall -g -c ex2.c
	 * Step 2: gcc -Wall -g -o ex2 ex2.o
	 * Step 3: ./ex2
	 *
	 * b)
	 *
	 * SIGNAL: SIGSTOP
	 * NUMBER: 19
	 *
	 * c)
	 *
	 * SIGNAL: SIGCONT
	 * NUMBER: 18
	 *
	 * d)
	 *
	 * SIGNAL: SIGKILL
	 * NUMBER: 9
	 *
	 *
	 * f) NOTA: Não percebi o que era para fazer. (VERIFICAR).
	 *
	 */

	// pl1b_ex2(); para correr o programa original
	// pl1b_ex2e(); // Para executar a funcionalidade pedida no exercício 2 e).

	return 0;

}
