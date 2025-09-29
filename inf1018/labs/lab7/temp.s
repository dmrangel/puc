/* #include <stdio.h>

int nums[4] = {65, -105, 111, 34};

int main (void) {
  int i=0;
  int s = 0;
  int indice=0;

  while(i!=4){
  indice= i*4;
  int *temp=nums;
  temp+=indice;
  soma+=*temp;
  i++;
}
  printf ("soma = %d\n", s);

  return 0;
}

*/

.data
nums:  .int  65, -105, 111, 34
Sf:  .string "%d\n"    # string de formato para printf

.text
.globl  main

main:

/********************************************************/
/* mantenha este trecho aqui e nao mexa - prologo !!!   */
  pushq   %rbp
  movq    %rsp, %rbp
  subq    $16, %rsp
  movq    %rbx, -8(%rbp)  /* guarda rbx */
  movq    %r12, -16(%rbp)  /* guarda r12 */
/********************************************************/

movq $0, %rbx /* i=0 */
movl $0, %r12d /* soma=0 */

inicio: 
cmpq $4, %rbx /* if(i==4) */
jge L1
movq %rbx, %r13 /* colocando i dentro de indice */
imulq $4, %r13
movq $nums, %r14 /* temp */
addq %r13, %r14
addl (%r14), %r12d /* soma+=temp */
addq $1, %rbx /* i++ */
jmp inicio

L1:
movl %r12d, %eax
/*************************************************************/
/* este trecho imprime o valor de %eax (estraga %eax)  */
  movq    $Sf, %rdi    /* primeiro parametro (ponteiro)*/
  movl    %eax, %esi   /* segundo parametro  (inteiro) */
  call  printf       /* chama a funcao da biblioteca */
/*************************************************************/

/***************************************************************/
/* mantenha este trecho aqui e nao mexa - finalizacao!!!!      */
  movq  $0, %rax  /* rax = 0  (valor de retorno) */
  movq  -8(%rbp), %rbx
  movq  -16(%rbp), %r12
  leave
  ret      
/***************************************************************/