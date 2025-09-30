/*
int fat (int edi) {
  if (edi==0) return 1;

  // else return edi*fat(edi-1);

  r12d = edi;
  edi = edi - 1;
  eax = fat(edi);
  eax = eax * r12d;
  return eax;
}
*/

.data

.text
.globl fat

fat:
    pushq %rbp
    movq %rsp, %rbp
    subq $8, %rsp
    movl %r12d, -8(%rbp)

    cmpl $0, %edi
    je end

    movl %edi, %r12d
    subl $1, %edi
    call fat
    imull %r12d, %eax

    movl -8(%rbp), %r12d
    leave
    ret

end:
    movl -8(%rbp), %r12d
    movl $1, %eax
    leave
    ret
