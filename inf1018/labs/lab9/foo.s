/*
void foo (int a[], int n) {
  int i = 0;
  int s = 0;
  while (i<n){
    s += *(a + i * 4)
    if (*(a + i * 4)==0){
        *(a + i * 4) = s;
        s = 0;
    }
    i++;
  }
}
*/

.data

.text
.globl foo

foo:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp
    movq %r12, -8(%rbp) # i
    movq %r13, -16(%rbp) # s
    movq %r14, -24(%rbp) # a
    movq %r15, -32(%rbp) # saved i
    
    movl $0, %r15d # saved i = 0
    movl $0, %r13d # s = 0

while:
    movslq %r15d, %r12 # i = saved i
    movq %rdi, %r14 # a = rdi
    
    cmpl %esi, %r15d # saved i < n
    jge end
    
    imulq $4, %r12 # i = i * 4
    addq %r12, %r14 # r14 = a + i * 4

    addl (%r14), %r13d # s += *(a + i * 4)

    cmpl $0, (%r14) # if *(a + i * 4) == 0
    je loop

    incl %r15d # saved i++
    jmp while

loop:
    movl %r13d, (%r14) # *(a + i * 4) == s 
    movl $0, %r13d # s = 0
    incl %r15d # saved i++
    jmp while

end:
    movq -8(%rbp), %r12
    movq -16(%rbp), %r13
    movq -24(%rbp), %r14
    movq -32(%rbp), %r15
    leave
    ret
