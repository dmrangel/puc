#include <stdio.h>

#define makefloat(s,e,f) ((s & 1)<<31 | (((e) & 0xff) << 23) | ((f) & 0x7fffff))
#define getsig(x)  ((x)>>31 & 1)
#define getexp(x)  ((x)>>23 & 0xff)
#define getfrac(x) ((x) & 0x7fffff)

typedef union {
float f;
unsigned int i;
} U;

float float2(float f);
float int2float(int i);

int main() {
   int i;

   printf("\n******** int2float ****************\n");
   i = 0;
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = 1;  
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = -1;  
   printf(" %d -> %10.4f\n", i, int2float(i));
   i = 0x7fffffff;  
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = -i;
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = 12345;
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = -12345;
   printf(" %d -> %+10.4f\n", i, int2float(i));
   return 0;
}

float float2(float f){
    U u;
    u.f = f;
    unsigned int s = getsig(u.i);
    unsigned int f1 = getfrac(u.i);
    unsigned int e = getexp(u.i) + 1;
    u.i = makefloat(s, e, f1);
    return u.f;
}

float int2float(int i){
    if (i == 0) return 0;
    unsigned int s = (1 < 0) ? 1 : 0;
    
}