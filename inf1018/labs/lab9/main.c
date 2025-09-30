#include <stdio.h>

int add (int a, int b, int c);
int fat (int n);
void foo(int a[], int n);

int main (void) {
   printf("%d\n", add(1,2,3));
   printf("%d\n", fat(5));
   int a[6] = {1,2,0,3,4,0};
   for (int i = 0; i<6; i++){
      printf("%d", a[i]);
   }
   foo(a, 6);
   printf("\n");
   for (int i = 0; i<6; i++){
      printf("%d", a[i]);
   }
   printf("\n");
   return 0;
}