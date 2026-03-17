#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_START(P) ((intptr_t)(P)&~(pagesize-1))
#define PAGE_END(P) (((intptr_t)(P)+pagesize-1)&~(pagesize-1))

/*
 * The execpage() function shall change the specified memory pages
 * permissions into executable.
 *
 * void *ptr  = pointer to start of memory buff
 * size_t len = memory buff size in bytes
 *
 * The function returns 0 if successful and -1 if any error is encountered.
 * errono may be used to diagnose the error.
 */
int execpage(void *ptr, size_t len) {
	int ret;

	const long pagesize = sysconf(_SC_PAGE_SIZE);
	if (pagesize == -1)
		return -1;

	ret = mprotect((void *)PAGE_START(ptr),
		 PAGE_END((intptr_t)ptr + len) - PAGE_START(ptr),
		 PROT_READ | PROT_WRITE | PROT_EXEC);
	if (ret == -1)
		return -1;

	return 0;
}

#undef PAGE_START
#undef PAGE_END

int add (int x) {
  return x+1;
}

unsigned char codigo[] = {0xe9, 0x0, 0x0, 0x0, 0x0, 0xc3};

typedef int (*funcp) (int x);

int main(void){
    execpage(codigo, sizeof(codigo));
    
    funcp f = (funcp)codigo;

    long deslocamento = (long)add - ((long)codigo + 5);
    codigo[1] = (unsigned char)deslocamento;
    codigo[2] = (unsigned char)(deslocamento >> 8);
    codigo[3] = (unsigned char)(deslocamento >> 16);
    codigo[4] = (unsigned char)(deslocamento >> 24);

    int i = (*f)(10);
    printf("valor inicial: 10, valor final: %d\n", i);
}