/*
Aluno: Davi Rangel
Turma: 3WB
Matrícula: 2411930
*/

#include <stdio.h>

typedef unsigned int Set;

Set set_remove(Set s, int n){
    unsigned int temp = 0x1 << n;
    s = s & (~temp);
    return s;
}

Set set_diferenca(Set s1, Set s2){
    s1 = s1 & (~s2);
    return s1;
}

int main(void){
    Set s1 = 0xF; // 1111
    Set s2 = 0x3; // 0011
    printf("set_remove: %X\n", set_remove(s1, 1)); // 0xD (1101)
    printf("set_diferenca: %X\n", set_diferenca(s1, s2)); // 0xC (1100)
    return 0;
}