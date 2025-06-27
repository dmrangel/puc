/* Davi_Rangel 2411930 3WB */

#ifndef PEQCOMP_H
#define PEQCOMP_H

#include <stdio.h>

// Define o tipo de ponteiro de funcao
typedef int (*funcp)(int, int, int);

// Prototipo do compilador
funcp peqcomp(FILE *f, unsigned char codigo[]);

#endif