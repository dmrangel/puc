#include <stdio.h>

unsigned char switch_byte(unsigned char x);
unsigned char rotate_left(unsigned char x, int n);

unsigned char switch_byte(unsigned char x){
    int metade1 = x << 4;
    int metade2 = x >> 4;
    return metade1 + metade2;
}

unsigned char rotate_left(unsigned char x, int n){
    // int prim;
    // for (int i = 0; i < n; i++){
    //     prim = x >> 7;
    //     x = (x << 1) + prim;
    // }
    // return x;

    int final = x >> (8 - n);
    return (x << n) + final;
}

int main(void){
    printf("n: ab | switch = %x\n", switch_byte(0xAB));
    printf("n = 0x61\n");
    printf("1 rotates left = %x\n", rotate_left(0x61, 1));
    printf("2 rotates left = %x\n", rotate_left(0x61, 2));
    printf("7 rotates left = %x\n", rotate_left(0x61, 7));
    return 0;
}