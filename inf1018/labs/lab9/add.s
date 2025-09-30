/*
int add (int a, int b, int c) {
  return a+b+c;
}
*/

.data

.text
.globl add

add:
  addl %edi, %esi
  addl %esi, %edx
  movl %edx, %eax

  ret
