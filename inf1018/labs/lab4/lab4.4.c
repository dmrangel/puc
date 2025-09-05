/* programa 2*/
#include <stdio.h>

void dump (void *p, int n) {
  unsigned char *p1 = p;
  while (n--) {
    printf("%p - %02x\n", p1, *p1);
    p1++;
  }
}

int main (void) {
  signed char sc = 0x80;
  unsigned int ui = sc;
  printf("signed char sc: %d | uint ui: %u\n", sc, ui);
  printf("Dump sc:\n");
  dump(&sc, 1);
  printf("Dump ui:\n");
  dump(&ui, 4);
  return 0;
}