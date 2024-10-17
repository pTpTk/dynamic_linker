.section .text
.global putchar
.type   putchar, @function

putchar:
    push %rbp
    mov  %rsp, %rbp

    push %rdi
    mov $1, %rax
    mov $1, %rdi
    mov %rsp, %rsi
    mov $1, %rdx
    syscall

    mov %rbp, %rsp
    pop %rbp
    ret

.global putchar2
.type   putchar2, @function

putchar2:
    push %rbp
    mov  %rsp, %rbp

    push %rdi
    mov $1, %rax
    mov $1, %rdi
    mov %rsp, %rsi
    mov $1, %rdx
    syscall

    mov %rbp, %rsp
    pop %rbp
    ret

.global putchar3
.type   putchar3, @function

putchar3:
    push %rbp
    mov  %rsp, %rbp

    push %rdi
    mov $1, %rax
    mov $1, %rdi
    mov %rsp, %rsi
    mov $1, %rdx
    syscall

    mov %rbp, %rsp
    pop %rbp
    ret
