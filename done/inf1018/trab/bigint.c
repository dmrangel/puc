  /* Davi_Rangel 2411930 3WB */
  /* Nome_do_Aluno2 Matricula Turma */

#include "bigint.h"

void big_val (BigInt res, long val){
    long temp = val;
    for (int i = 0; i < 8; i++){
        res[i] = temp & 0xFF;
        temp = temp>>8;
    }
    unsigned char extensao = 0;
    if ((res[7] & 0x80)>>7 == 1) extensao = 0xFF;
    for (int i = 8; i < 16; i++){
        res[i] = extensao;
    }
}