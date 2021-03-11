#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main(void) {
    fork();
    fork();
    fork();
    printf("SCOMP!\n");
}


// RESPOSTAS:
// a) Foram criados 8 processos, como é um fork que não foi específico irá criar cópias seguindo
//    a expressão 2^n.
// b)
//
//		------------------------------------
//		|									|
//		|									|
//		|									|
//		|									|
//		.---------------					.---------------
//		|				|					|				|
//		|				|					|				|
//		|				|					|				|
//		|				|					|				|
//		.--------		.--------			.--------		.-------
//		|		|		|		|			|		|		|		|
//		|		|		|		|			|		|		|		|
//		|		|		|		|			|		|		|		|
//		|		|		|		|			|		|		|		|
//
//	c) Irá aparecer 8 vezes printado a palavra SCOMP.
