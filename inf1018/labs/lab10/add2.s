/*
int add2 (struct X *x) {
  if (x == NULL) return 0;
  else return x->val + add2(x->next);
}
*/

.text

.data
.globl add

add:
pushq %rbp
movq %rsp, %rbp
subq $8, %rsp
movq %r12, -8(%rbp)
cmpq $0, %rdi
je L1
movl (%rdi), %r12d
movq 8(%rdi), %rdi
call add
addl %r12d, %eax
jmp fim

L1:
movl $0, %eax

fim:
movq -8(%rbp), %r12
leave
ret
