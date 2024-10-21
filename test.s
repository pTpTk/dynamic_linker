.globl _start
_start:
 xor   %rbp, %rbp
 call  main
 movl  %eax, %edi
 movl  $60, %eax
 syscall

 .globl main
main:
 push	%rbp
 movq	%rsp, %rbp
 movl	$72, %eax
 movl	%eax, %edi
 call	putchar2
 movl	$101, %eax
 movl	%eax, %edi
 call	putchar
 movl	$108, %eax
 movl	%eax, %edi
 call	putchar3
 movl	$108, %eax
 movl	%eax, %edi
 call	putchar
 movl	$111, %eax
 movl	%eax, %edi
 call	putchar
 movl	$44, %eax
 movl	%eax, %edi
 call	putchar
 movl	$32, %eax
 movl	%eax, %edi
 call	putchar
 movl	$87, %eax
 movl	%eax, %edi
 call	putchar
 movl	$111, %eax
 movl	%eax, %edi
 call	putchar
 movl	$114, %eax
 movl	%eax, %edi
 call	putchar
 movl	$108, %eax
 movl	%eax, %edi
 call	putchar
 movl	$100, %eax
 movl	%eax, %edi
 call	putchar
 movl	$33, %eax
 movl	%eax, %edi
 call	putchar
 movl	$10, %eax
 movl	%eax, %edi
 call	putchar
 movl	$0, %eax
 movq	%rbp, %rsp
 pop	%rbp
 ret
