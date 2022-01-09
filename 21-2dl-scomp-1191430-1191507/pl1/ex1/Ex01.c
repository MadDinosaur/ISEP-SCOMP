#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main(void) {
    int x = 1;
    pid_t p = fork(); /*pid_t: sys/types.h; fork(): unistd.h*/
    if (p == 0) { // Filho
        x = x+1;
        printf("1. x = %d\n", x);
    } else { // Pai
        x = x-1;
        printf("2. x = %d\n", x);
    }
    printf("3. %d; x = %d\n", p, x);
}


// RESPOSTAS:
// a) OUTPUT:
//    2. x = 0;
//    3. 1234; x = 0;
//    1. x = 2;
//    3. 0; x = 2;

// b) Falso, é impossível saber quem será executado primeiro, terá a haver com o escalonamento do processador.
