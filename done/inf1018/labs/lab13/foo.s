/*
int foo (int x) {
  return add(x);
}
*/

.text
.globl foo

foo:
jmp add

ret
