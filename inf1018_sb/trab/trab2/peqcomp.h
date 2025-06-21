#include <stdio.h>

typedef int (*funcp)(int, int, int);
funcp peqcomp(FILE *f, unsigned char codigo[]);