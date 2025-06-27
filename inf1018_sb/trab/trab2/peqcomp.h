/* Davi_Rangel 2411930 3WB */

#ifndef PEQCOMP_H
#define PEQCOMP_H

#include <stdio.h>

// Define um tipo para um ponteiro de função que recebe até 3 inteiros.
typedef int (*funcp)(int, int, int);

// Protótipo da função do compilador.
funcp peqcomp(FILE *f, unsigned char codigo[]);

#endif