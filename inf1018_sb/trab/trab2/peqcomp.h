/* Davi_Rangel 2411930 3WB */

#ifndef PEQCOMP_H
#define PEQCOMP_H

#include <stdio.h>

// Define um tipo pra um ponteiro de funcao que recebe ate 3 inteiros.
typedef int (*funcp)(int, int, int);

// Prototipo da funcao do compilador.
funcp peq_compila(FILE *f, unsigned char codigo_maquina[]);

#endif