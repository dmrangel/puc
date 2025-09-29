#include <stdio.h>

int main(void){
    int n = 1;
    while(n <= 10){
        printf("quadrado de %d: %d\n", n, n*n);
        n++;
    }
    return 0;
}