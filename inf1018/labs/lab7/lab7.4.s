/*
#include <stdio.h>

int nums[4] = {65, -105, 111, 34};

int main (void) {
  int i;
  int s = 0;
  int *temp = nums

  i = 0;
  while (i < 4){
    temp = nums
    temp += i * 4
    s += *temp
    i++;
  }

  printf ("soma = %d\n", s);

  return 0;
}
*/

.data
Sf: .string "soma = %d\n"
Nums: .int 65, -105, 111, 34

.text
.globl main

main:

/********************************************************/
/* mantenha este trecho aqui e nao mexa - prologo !!!   */
  pushq   %rbp
  movq    %rsp, %rbp
  subq    $16, %rsp
  movq    %rbx, -8(%rbp)  /* guarda rbx */
  movq    %r12, -16(%rbp)  /* guarda r12 */
/********************************************************/

movl $0, %ebx /* s = 0 */
movq $0, %r12 /* i = 0 */

L1:
cmpq $4, %r12 /* i == 4 */
je L2

movq %r12, %r14 /* indice = i */
imulq $4, %r14 /* indice *= 4 */

movq $Nums, %r13 /* temp = nums */
addq %r14, %r13 /* *temp += indice */

addl (%r13), %ebx /* s += *temp */
addq $1, %r12 /* i++*/

jmp L1

L2:
movl %ebx, %eax
/*************************************************************/
/* este trecho imprime o valor de %eax (estraga %eax)  */
  movq    $Sf, %rdi    /* primeiro parametro (ponteiro)*/
  movl    %eax, %esi   /* segundo parametro  (inteiro) */
  movl  $0, %eax
  call  printf       /* chama a funcao da biblioteca */
/*************************************************************/

/***************************************************************/
/* mantenha este trecho aqui e nao mexa - finalizacao!!!!      */
  movq  $0, %rax  /* rax = 0  (valor de retorno) */
  movq    -16(%rbp), %r12 /* recupera r12 */
  movq    -8(%rbp), %rbx  /* recupera rbx */
  leave
  ret      
/***************************************************************/