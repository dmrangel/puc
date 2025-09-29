#include <stdio.h>
#define LIM 1

int filtro(int x, int lim);

int nums[5] = {3,-5,7,8,-2};

int main() {
  int i;
  int *p = nums;
//   for (i = 0; i < 5; i++) {
//     printf("%d\n", filtro(*p,LIM));
//     p++;
//   }
  i = 0;
  while(i<5){
    printf("%d\n", filtro(*p,LIM));
    i++;
    p++;
  }
  return 0;
}