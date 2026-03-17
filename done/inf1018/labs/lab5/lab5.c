#include <stdio.h>

void dump (void *p, int n) {
  unsigned char *p1 = p;
  while (n--) {
    printf("%p - %02x\n", p1, *p1);
    p1++;
  }
}

struct X {
  int a;
  short b;
  int c;
} x = {0xa1a2a3a4, 0xb1b2, 0xc1c2c3c4};


// 12 bytes - c1,pp,pp,pp,i,i,i,i,c2,pp,pp,pp
struct X1 {
  char c1;
  int i;
  char c2;
} x1;

// 16 bytes - l,l,l,l,l,l,l,l,c,pp,pp,pp,pp,pp,pp,pp
struct X2 {
  long l;
  char c;
} x2;

// 8 bytes - i,i,i,i,c1,c2,pp,pp
struct X3 {
  int i;
  char c1;
  char c2;
} x3;

// 24 bytes - x*16,c,pp*7
struct X4 {
  struct X2 x;
  char c;
} x4;

// 3 bytes - c1, c2, c3
struct X5 {
  char c1;
  char c2;
  char c3;
} x5;

// 16 bytes - s1,s1,pp,pp,i,i,i,i,c[0],c[1],c[2],pp,s2,s2,pp,pp
struct X6 {
  short s1;
  int i;
  char c[3];
  short s2;
} x6;

// 8 bytes - i/c[0],i/c[1],i/c[2],i/c[3],c[4],pp,pp,pp
union U1 {
  int i;
  char c[5];
} u1;

// 6 bytes - s/c[0],s/c[1],c[2],c[3],c[4],pp
union U2 {
  short s;
  char c[5];
} u2;


int main (void) {
//   int i,j;
//   short a[2][3];
//   int b[2];

//   for (i=0;i<2;i++) {
//     b[i] = i;
//     for (j=0;j<3;j++)
//       a[i][j] = 3*i+j;
//   }

//   printf ("b: \n");
//   dump (b, sizeof(b));
//   printf ("a: \n");
//   dump (a, sizeof(a));
//   printf ("x: \n");
  dump (&u1, sizeof(u1));

  return 0;
}