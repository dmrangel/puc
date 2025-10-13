/*
struct X {
  int val1;
  int val2;
};

int f(int i, int v);

void boo (struct X *px, int n, int val) {
  while (n!=0) {
    px->val2 = f(px->val1, val);
    px++;
    n--;
  }
}

px -> r12
n -> r13d
*/
.text
.globl boo

boo:
pushq %rbp
movq %rsp, %rbp
subq $16, %rsp
movq %r12, -8(%rbp)
movq %r13, -16(%rbp)

while:
cmpl $0, %esi
je fim

movq %rdi, %r12
movl %esi, %r13d
movl (%r12), %edi
movl %edx, %esi
call f
movl %r13d, %esi
movq %r12, %rdi
movl %eax, 4(%rdi)

addq $8, %rdi
decl %esi
jmp while

fim:
movq -8(%rbp), %r12
movq -16(%rbp), %r13
leave
ret
