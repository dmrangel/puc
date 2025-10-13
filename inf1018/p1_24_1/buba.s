/*
(b) (3,5 pontos)
int buba(char *v, int n, char c){
    int i = 0, s = 0;
    while(i < n){
        temp = v + i
        if (*temp <= c)
        s += corta(temp, n);
        i++;
    }
    return s;
}

i -> r12d
s -> r13d
temp -> r14
v saved -> r15
*/

.data

.text
.globl buba

buba:
pushq %rbp
movq %rsp, %rbp
subq $32, %rsp
movq %r12, -8(%rbp)
movq %r13, -16(%rbp)
movq %r14, -24(%rbp)

movl $0, %r12d # i = 0
movl $0, %r13d # s = 0

while:
cmpl %esi, %r12d
jge fim # if i >= n

movslq %r12d, %r14 # temp = i
addq %rdi, %r14 # temp += v
cmpb %dl, (%r14)
jle L1 # if *temp <= c
jmp L2 # if not

L1:
pushq %rdi # saving v
movq %r14, %rdi # 1st arg = temp
call corta
addl %eax, %r13d # s += corta(temp, n)
popq %rdi # restore v

L2:
incl %r12d # i++
jmp while

fim:
movl %r13d, %eax # return s

movq -8(%rbp), %r12 
movq -16(%rbp), %r13
movq -24(%rbp), %r14

leave
ret
