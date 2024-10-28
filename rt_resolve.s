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
 *   push func params
 *   call runtime_resolve
 *
 * runtime_resolve :
 *   resolve symbol
 *   ret _runtime_resolve
 *
 * _runtime_resolve :
 *   pop params
 *   jump func
 *
 * func :
 *   ...
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

    # restore func params and jump to func
    pop %r9
    pop %r8
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi

    jmp *%rax
