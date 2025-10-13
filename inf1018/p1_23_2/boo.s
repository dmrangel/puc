/*
struct S
{
    int v;
    struct S *prox;
};
extern int norma[];
int boo(struct S *s, int n){
    int acum = 0;
    while (s){
        s->v = acerta(norma, n + acum);
        acum += n;
        s = s->prox;
    }
    return acum;
}

acum -> r12d
*/

.data

.text
.globl boo

boo:
pushq %rbp
movq %rsp, %rbp
subq $32, %rsp
movq %r12, -8(%rbp)
movq %r13, -16(%rbp)
movq %r14, -24(%rbp)

movl $0, %r12d # int acum = 0
movq $norma, %r13

while:
cmpq $0, %rdi
je fim # if s == 0

movq %rdi, %r13 # saving s
movq %rsi, %r14 # saving n
movq $norma, %rdi # 1st arg = norma
addl %r12d, %esi # 2nd arg = n + acum
call acerta
movq %r14, %rsi # restore n
movq %r13, %rdi # restore s
movl %eax, (%rdi) # s->v = acerta(norma, n + acum)
addl %esi, %r12d # acum += n
movq 8(%rdi), %rdi # s = s->prox

jmp while

fim:
movl %r12d, %eax
movq -8(%rbp), %r12
movq -16(%rbp), %r13
movq -24(%rbp), %r14
leave
ret