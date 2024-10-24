.section .text
.align	16
.global _runtime_resolve
.type   _runtime_resolve, @function

/* main :
 *   call .plt entry
 *
 * .plt :
 *   push params
 *   jump _runtime_resolve
 *
 * _runtime_resolve :
 *   move params to regs
 *   jump runtime_resolve
 *
 * runtime_resolve :
 *   resolve symbol
 *   call func
 *
 * func :
 *   ...
 *   ret runtime_resolve
 *
 * runtime_resolve :
 *   ret main
 */

_runtime_resolve:
    # temporarily save resolve fn params
    pop %rax
    pop %r11

    # save func params 
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %r8
    push %r9

    # load resolve params and call resolve
    mov %rax, %rdi
    mov %r11, %rsi

    call runtime_resolve



    jmp runtime_resolve
 jmp hello
