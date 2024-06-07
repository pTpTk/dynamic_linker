.globl _start
_start:
 xor   %rbp, %rbp
 call  main
 movl  %eax, %edi
 movl  $60, %eax
 syscall

.globl main
main:
 push   %rbp
 movq   %rsp, %rbp
 movl   $2, %eax
 movq   %rbp, %rsp
 pop    %rbp
 ret