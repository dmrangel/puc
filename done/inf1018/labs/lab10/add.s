/*
struct X {
  int val;
  struct X *next;
};

int add (struct X *x) {
  int a = 0;
  while(x != NULL){
    a += x->val;
    x = x->next;
  }
  return a;
}
*/

.data

.text
.globl add

add:
pushq %rbp
movq %rsp, %rbp
subq $16, %rsp
movq %r12, -8(%rbp)

movl $0, %r12d

while:
cmpq $0, %rdi
je fim

addl (%rdi), %r12d
movq 8(%rdi), %rdi
jmp while

fim:
movl %r12d, %eax
movq -8(%rbp), %r12
leave
ret
