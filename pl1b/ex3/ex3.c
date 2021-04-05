#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

void sig_handler(int sig)
{
	printf("Handler:");
    printf("SIGUSR1 has been captured and was sent by the process with PID number: %d\n", getpid());
}

int pl1b_ex3c()
{
    signal(SIGUSR1, sig_handler);

    for(;;)
	{
		printf("I Love SCOMP!\n");
		sleep(1);
	}
    return 0;
}

int main() {

	/* EX3:
	 *
	 * a)
	 *
	 * Sigaction, porque se utilizarmos o signal para salvar e for necessário
	 * restabelecer uma action, pode não ser possível estabelecer adequadamente
	 * o handler que foi estabelecido com a sigaction, enquanto que o sigaction
	 * consegue o fazer mesmo se a ação for estabelecida por um signal ou sigaction.
	 *
	 * b)
	 *
	 * Função: int sigaction(int sig,const struct sigaction * act,struct sigaction * oact)
	 *
	 * Em que:
	 * int sig -> signal;
	 * act -> sets the action details for signal sig;
	 * oact -> (if not NULL) is used to store the previously set action details.
	 *
	 * Possíveis valores:
	 * sig -> 1-64;
	 * act -> siginfo_t;
	 * oact -> NULL;
	 *
	 * d)
	 *
	 * Signal handlers have a problem accessing static data structures.
	 * From this, you can get corrupted data and such.
	 * If you try and call printf() within a signal handler, many times you will get strange output.
	 * The same is for global variables unless you use atomic types.
	 *
	 * e)
	 *
	 * LIST OF ALL FUCTIONS THAT CAN BE SAFELY USED WITH SIGNAL HANDLER:
	 *
	 * abort(3)
     * accept(2)
     * access(2)
     * aio_error(3)
     * aio_return(3)
     * aio_suspend(3)
     * alarm(2)
     * bind(2)
     * cfgetispeed(3)
     * cfgetospeed(3)
     * cfsetispeed(3)
     * cfsetospeed(3)
     * chdir(2)
     * chmod(2)
     * chown(2)
     * clock_gettime(2)
     * close(2)
     * connect(2)
     * creat(2)
     * dup(2)
     * dup2(2)
     * execl(3)
     * execle(3)
     * execv(3)
     * execve(2)
     * _exit(2)
     * _Exit(2)
     * faccessat(2)
     * fchdir(2)
     * fchmod(2)
     * fchmodat(2)
     * fchown(2)
     * fchownat(2)
     * fcntl(2)
     * fdatasync(2)
     * fexecve(3)
     * ffs(3)
     * fork(2)
     * fstat(2)
     * fstatat(2)
     * fsync(2)
     * ftruncate(2)
     * futimens(3)
     * getegid(2)
     * geteuid(2)
     * getgid(2)
     * getgroups(2)
     * getpeername(2)
     * getpgrp(2)
     * getpid(2)
     * getppid(2)
     * getsockname(2)
     * getsockopt(2)
     * getuid(2)
     * htonl(3)
     * htons(3)
     * kill(2)
     * link(2)
     * linkat(2)
     * listen(2)
     * longjmp(3)
     * lseek(2)
     * lstat(2)
     * memccpy(3)
     * memchr(3)
     * memcmp(3)
     * memcpy(3)
     * memmove(3)
     * memset(3)
     * mkdir(2)
     * mkdirat(2)
     * mkfifo(3)
     * mkfifoat(3)
     * mknod(2)
     * mknodat(2)
     * ntohl(3)
     * ntohs(3)
     * open(2)
     * openat(2)
     * pause(2)
     * pipe(2)
     * poll(2)
     * posix_trace_event(3)
     * pselect(2)
     * pthread_kill(3)
     * pthread_self(3)
     * pthread_sigmask(3)
     * raise(3)
     * read(2)
     * readlink(2)
     * readlinkat(2)
     * recv(2)
     * recvfrom(2)
     * recvmsg(2)
     * rename(2)
     * renameat(2)
     * rmdir(2)
     * select(2)
     * sem_post(3)
     * send(2)
     * sendmsg(2)
     * sendto(2)
     * setgid(2)
     * setpgid(2)
     * setsid(2)
     * setsockopt(2)
     * setuid(2)
     * shutdown(2)
     * sigaction(2)
     * sigaddset(3)
     * sigdelset(3)
     * sigemptyset(3)
     * sigfillset(3)
     * sigismember(3)
     * siglongjmp(3)
     * signal(2)
     * sigpause(3)
     * sigpending(2)
     * sigprocmask(2)
     * sigqueue(2)
     * sigset(3)
     * sigsuspend(2)
     * sleep(3)
     * sockatmark(3)
     * socket(2)
     * socketpair(2)
     * stat(2)
     * stpcpy(3)
     * stpncpy(3)
     * strcat(3)
     * strchr(3)
     * strcmp(3)
     * strcpy(3)
     * strcspn(3)
     * strlen(3)
     * strncat(3)
     * strncmp(3)
     * strncpy(3)
     * strnlen(3)
     * strpbrk(3)
     * strrchr(3)
     * strspn(3)
     * strstr(3)
     * strtok_r(3)
     * symlink(2)
     * symlinkat(2)
     * tcdrain(3)
     * tcflow(3)
     * tcflush(3)
     * tcgetattr(3)
     * tcgetpgrp(3)
     * tcsendbreak(3)
     * tcsetattr(3)
     * tcsetpgrp(3)
     * time(2)
     * timer_getoverrun(2)
     * timer_gettime(2)
     * timer_settime(2)
     * times(2)
     * umask(2)
     * uname(2)
     * unlink(2)
     * unlinkat(2)
     * utime(2)
     * wait(2)
     * waitpid(2)
     * wcpcpy(3)
     * wcpncpy(3)
     * wcscat(3)
     * wcschr(3)
     * wcscmp(3)
     * wcscpy(3)
     * wcscspn(3)
     * wcslen(3)
     * wcsncat(3)
     * wcsncmp(3)
     * wcsncpy(3)
     * wcsnlen(3)
     * wcspbrk(3)
     * wcsrchr(3)
     * wcsspn(3)
     * wcsstr(3)
     * wcstok(3)
     * wmemchr(3)
     * wmemcmp(3)
     * wmemcpy(3)
     * wmemmove(3)
     * wmemset(3)
     * write(2)
	 */

	// pl1b_ex3c(); // Para executar a funcionalidade pedida no exercício 3 c).

	return 0;
}
