/*
 * peqcomp.h
 * Define o protótipo da função peqcomp e o tipo funcp.
 */
#ifndef PEQCOMP_H
#define PEQCOMP_H

#include <stdio.h>

/*
 * Define um ponteiro para uma função que recebe até 3 inteiros
 * e retorna um inteiro.
 */
typedef int (*funcp)(int, int, int);

/*
 * Assinatura da função do micro-compilador.
 * f: arquivo fonte SBas aberto para leitura.
 * codigo: buffer onde o código de máquina será gerado.
 */
funcp peqcomp(FILE *f, unsigned char codigo[]);

#endif /* PEQCOMP_H */