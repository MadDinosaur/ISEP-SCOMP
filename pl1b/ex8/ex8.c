#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

volatile sig_atomic_t child_count_exited = 0;

void handler_signal(int sig) {
    child_count_exited++;
}

int main() {

	int status;
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask);
	act.sa_handler = handler_signal;
	sigaction(SIGUSR1, &act, NULL);

	pid_t p;

	for (int i = 0; i < 5; ++i)
	{
		p = fork();

		if (p == 0)
		{
			for (int j = i * 200; j < (i + 5) * 200; ++j)
			{
				printf("Number: %d\n", j);
			}

			printf("Process child number: %d\n", i);

			kill(getppid(), SIGUSR1);
			exit(i);

		}

		else if (p > 0)
		{
			pause();

			printf("Processes child exited. Count Number: %d\n", child_count_exited);

			for (int j = 0; j < 5; ++j)
			{
				wait(&status);
			}

			printf("Shutting down the program...\n");

		}

		else
		{
			printf("ERROR.\n");
		}
	}

	return 0;
}
