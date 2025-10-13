#include <stdio.h>

void dump (void *p, int n) {
    unsigned char *p1 = (unsigned char *) p;
    while (n--) {
        printf("%p - %02x\n", p1, *p1);
        p1++;
    }
}

struct S {
    int a;
    char c;
    char *p;
};

unsigned int inverteOrdemBytes(unsigned int i){
    unsigned int invertido = 0, temp;
    for (int n = 24; n >= 0; n= n-8){
        temp = i & 0xFF;
        temp = temp << n;
        invertido += temp;
        i = i >> 8;
    }

    return invertido;
}

int main(void) {
    struct S s;
    s.a = -16;
    s.c = 'd';
    s.p = &s.c;
    dump(&s, sizeof(struct S));

    unsigned int i = 0x01020304;
    unsigned int r = inverteOrdemBytes(i);
    printf("Original: 0x%08x, Invertido: 0x%08x\n", i, r);

    return 0;
}
