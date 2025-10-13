/*
(a) (1,5 pontos)
int corta(char c1, int b){
    return c1 + b;
}
*/

.data

.text
.globl corta

corta:
movsbl %dil, %eax
addl %esi, %eax

ret
