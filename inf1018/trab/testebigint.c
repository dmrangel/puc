  /* Davi_Rangel 2411930 3WB */
  /* Nome_do_Aluno2 Matricula Turma */

#include <stdio.h>
#include "bigint.h"

void big_val (BigInt res, long val);

void exibe_bigint(BigInt n, long val){
    printf("valor val: %ld\tvalor res: ", val);
    for (int i = 0; i < 16; i++){
        printf("%.2x ", n[i]);
    }
    printf("\n");
}

int main(void){
    // teste big_val
    // bigint 1
    printf("teste big_val\n");
    long val = 1;
    BigInt n;
    big_val(n, val);
    exibe_bigint(n, val);

    // bigint 255
    val = 255;
    big_val(n, val);
    exibe_bigint(n, val);


    // teste big_comp2
    // bigint 1
    printf("\nteste big_comp2\n");
    long val = 1;
    big_val(n, val);
    BigInt n_invertido;
    bug_comp2(n_invertido, n);
    exibe_bigint(n, val);

    return 0;
}

