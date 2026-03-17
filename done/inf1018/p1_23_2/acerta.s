/*
int acerta (int u[], int i){
    return *(u + (i * 4))
}
*/

.data

.text
.globl acerta

acerta:
pushq %rbp
movq %rsp, %rbp
subq $16, %rsp
movq %r12, -8(%rbp)
movq %r13, -16(%rbp)

movl %esi, %r12d
imull $4, %r12d
movslq %r12d, %r13
addq %rdi, %r13
movl (%r13), %eax

movq -8(%rbp), %r12
movq -16(%rbp), %r13
leave
ret