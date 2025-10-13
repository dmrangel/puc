/*
int f(int x);

void map2 (int* um, int * outro, int n) {
  int i = 0;
  while (i<n){
    *(outro+i) = f(*(um+i));
    i++;
  }
*/
.text
.globl map2

map2:
pushq %rbp
movq %rsp, %rbp
subq $32, %rsp
movq %r12, -8(%rbp)
movq %r13, -16(%rbp)
movq %r14, -24(%rbp)

movl $0, %r12d

while:
cmpl %edx, %r12d
jge fim

movslq %r12d, %r13 # r13 = i
imulq $4, %r13 # i *= 4
movq %rdi, %r14 # saving rdi
addq %r13, %rdi # rdi += i
addq %rsi, %r13 # r13 += rsi
movl (%rdi), %edi # 1st arg = *(um+i)
call f
movl %eax, (%r13) # *(outro+i) = f(*(um+i))
movq %r14, %rdi # restore rdi
incl %r12d # i++
jmp while

fim:
movq -8(%rbp), %r12
movq -16(%rbp), %r13
movq -24(%rbp), %r14
leave
ret
