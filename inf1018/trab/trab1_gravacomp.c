/* Davi_Rangel 2411930 3WB */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int gravacomp(int nstructs, void* valores, char* descritor, FILE* arquivo) {
    fputc(nstructs, arquivo);
    char* p = (char*)valores;
    int tam_struct = 0;
    for (int i = 0; descritor[i]; ) {
        if (descritor[i] == 'i' || descritor[i] == 'u') {
            tam_struct = (tam_struct + 3) & ~3;
            tam_struct += 4;
            i++;
        } else if (descritor[i] == 's') {
            int len = (descritor[i+1] - '0') * 10 + (descritor[i+2] - '0');
            tam_struct += len;
            i += 3;
        }
    }
    for (int s = 0; s < nstructs; s++) {
        int pos = 0;
        int ultimo;
        for (int i = 0; descritor[i]; ) {
            if (descritor[i] == 's') {
                if (descritor[i+3] == '\0') {
                    ultimo = 1;
                } else {
                    ultimo = 0;
                }
            }
            else{
                if (descritor[i+1] == '\0') {
                    ultimo = 1;
                } else {
                    ultimo = 0;
                }
            }
            if (descritor[i] == 'i' || descritor[i] == 'u') {
                pos = (pos + 3) & 0xFC;
                unsigned int val;
                memcpy(&val, p + s * tam_struct + pos, 4);
                int nbytes = 4;
                if (descritor[i] == 'i'){
                    int tmp = (int)val;
                    if (tmp >= -128 && tmp <= 127){
                        nbytes = 1;
                    } else if (tmp >= -32768 && tmp <= 32767){
                        nbytes = 2;
                    } else if (tmp >= -8388608 && tmp <= 8388607){
                        nbytes = 3;
                    }
                }
                else {
                    if (val <= 255){
                        nbytes = 1;
                    } else if (val <= 65535){
                        nbytes = 2;
                    } else if (val <= 16777215){
                        nbytes = 3;
                    }
                }
                unsigned char cab = 0;
                if (ultimo){
                    cab = cab | 0x80;
                }
                if (descritor[i] == 'i'){
                    cab = cab | 0x20;
                }
                cab = cab | nbytes;
                if (fputc(cab, arquivo) == EOF){
                    return -1;
                }
                for (int b = nbytes - 1; b >= 0; b--) {
                    fputc((val >> (b * 8)) & 0xFF, arquivo);
                }
                pos += 4;
                i++;
            }
            else if (descritor[i] == 's') {
                int len = (descritor[i+1] - '0') * 10 + (descritor[i+2] - '0');
                char* str = p + s * tam_struct + pos;
                int real_len = strlen(str);
                unsigned char cab = 0;
                if (ultimo){
                    cab = cab | 0x80;
                }
                cab = cab | 0x40;
                cab = cab | real_len;
                fputc(cab, arquivo);
                for (int b = 0; b < real_len; b++) {
                    fputc(str[b], arquivo);
                }
                pos += len;
                i += 3;
            }
        }
    }
    return 0;
}

void mostracomp (FILE *arquivo){
    int nstructs = fgetc(arquivo);
    printf("Estruturas: %d\n\n", nstructs);
    for (int i = 0; i < nstructs; i++){
        int ultimo = 0;
        while (ultimo == 0){
            int cab = fgetc(arquivo);
            int tipo = cab & 0x60;
            int tamanho = cab & 0x1F;
            ultimo = cab & 0x80;
            if (tipo == 0x40){
                char str[256];
                for (int n = 0; n < tamanho; n++){
                    str[n] = fgetc(arquivo);
                }
                str[tamanho] = '\0';
                printf("(str) %s\n", str);
            }
            else{
                unsigned int val = 0;
                for (int n = 0; n < tamanho; n++){
                    val = (val << 8) | fgetc(arquivo);
                }
                if (tipo == 0x20){
                    if (val & (1 << ((tamanho * 8) -1))){
                        val = val | (0xFFFFFFFF << (tamanho * 8));
                    }
                    printf("(int) %d (%08x)\n", (int)val, val);
                }
                else if (tipo == 0x00){
                    printf("(uns) %u (%08x)\n", val, val);
                }
            }
        }
        printf("\n");
    }
}