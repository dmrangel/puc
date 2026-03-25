#include <stdio.h>

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        FILE* arq = fopen(argv[i], "r"); if (arq == NULL) return 1;
        char c;
        while ((c = fgetc(arq)) != EOF) {
            printf("%c", c);
        }
        fclose(arq);
    }
    printf("\n");
    return 0;
}
