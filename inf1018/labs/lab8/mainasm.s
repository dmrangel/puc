  .data
  nums: .int 3, -5, 7, 8, -2
  s1:   .string "%d\n"

  .text
  .globl main
  main:
  /* prologo */
     pushq %rbp
     movq  %rsp, %rbp
     subq  $16, %rsp
     movq  %rbx, -8(%rbp)
     movq  %r12, -16(%rbp)

  /* coloque seu codigo aqui */
    movq $0, %rbx /* int i = 0 */
    movq $nums, %r12 /* %r12 = nums (int *p = nums) */
L1:
    cmpq $5, %rbx /* if i == 5 */
    je L2 /* end while */
    movl (%r12), %edi /* arg1 filtro = *p */
    movl $1, %esi /* arg2 filtro = 1 */
    call filtro
    movq $s1, %rdi /* arg1 printf */
    movl %eax, %esi /* arg2 printf = ret filtro */
    call printf
    addq $1, %rbx /* i++ */
    addq $4, %r12 /* p++ */
    jmp L1

L2:
  /* finalizacao */
     movq -8(%rbp), %rbx
     movq -16(%rbp), %r12
     leave
     ret
