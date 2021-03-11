#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    alineaA();
    int a=0, b, c, d;
    b = (int) fork();
    c = (int) getpid(); /* getpid(), getppid(): unistd.h*/
    d = (int) getppid();
    a = a + 5;
    
    printf("\na=%d, b=%d, c=%d, d=%d\n",a,b,c,d);
} 

void alineaA (void) {
    printf("a = 5 em ambos os processos\n");
    printf("b (parent) = c (child)\n");
    printf("c (parent) = d (child)\n");
}