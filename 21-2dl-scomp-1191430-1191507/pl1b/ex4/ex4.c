#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{

	/* EX4:
	 *
	 * TABLE:
	 *
	 * |     Signal     |   Number   |                   Action                   |
	 * ----------------------------------------------------------------------------
	 * 	     SIGHUP            1       Hangup detected on controlling terminal
	 *       SIGINT            2       Interrupt from keyboard
	 *       SIGQUIT	       3       Quit from keyboard
	 *       SIGILL	           4       Illegal Instruction
	 *       SIGTRAP           5       Trace/breakpoint trap
	 *       SIGABRT           6       Abort signal from abort(3)
	 *       SIGFPE	           8       Floating-point exception
	 *       SIGKIL            9       Kill signal
	 *       SIGUSR1           10      SIGUSR1
	 *       SIGSEGV           11      Invalid memory reference
	 *       SIGUSR2           12      User-defined signal 2
	 *       SIGPIPE           13      Broken pipe: write to pipe with no
	 *       SIGALRM           14      Timer signal from alarm(2)
	 *       SIGTERM           15      Termination signal
	 *       SIGCHLD           17      Child stopped or terminated
	 *       SIGCONT           18      Continue if stopped
	 *       SIGSTOP           19      Stop process
	 *       SIGTSTP           20      Stop typed at terminal
	 *       SIGTTIN	       21      Terminal input for background process
	 *       SIGTTOU           22      Terminal output for background process
	 *
	 */

	return 0;
}
