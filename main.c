#include <stdlib.h>
#include <stddef.h>
#include <fcntl.h>
#include <elf.h>

#include "elf.h"
#include "syscall.c"

int main(int argc, char ** argv, char ** envp) {
    size_t * sp = (size_t *)(argv - 1);
    size_t * auxvals = (size_t *)envp;
    while (*auxvals++);

    void * entry = NULL;
    PHDR * phdr = NULL;
    for (size_t i = 0; auxvals[i] != AT_NULL; i += 2) {
        size_t val = auxvals[i + 1];
        switch (auxvals[i]) {
            case AT_ENTRY:
                entry = (void *)val;
                break;
            case AT_PHDR:
                phdr = (PHDR *)val;
                break;
        }
    }

    // handling program headers
    PHDR * ph_ptr = phdr;
    int ph_size = phdr->p_memsz;
    int ph_count = ph_size / 0x38;
    for(int i = 1; i < ph_count; ++i) {
        ++ph_ptr;
        switch(ph_ptr->p_type) {
            case 0x03: // INTERP
                write2(1, "INTERP\n", 7);
                break;
            case 0x01: // LOAD
                write2(1, "LOAD\n", 5);
                break;
            case 0x02: // DYNAMIC
                write2(1, "DYNAMIC\n", 8);
        }
    }


    int fd = open2("foo.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644); 
    
    int sz = write2(fd, "hello geeks\n", 12); 
    
    close2(fd); 

    sp[-1] = (size_t)entry;

    asm(
        "mov %[sp], %%rsp\n\t"
        "mov $0, %%rax\n\t"
        "mov $0, %%rbx\n\t"
        "mov $0, %%rcx\n\t"
        "mov $0, %%rdx\n\t"
        "mov $0, %%rdi\n\t"
        "mov $0, %%rsi\n\t"
        "mov $0, %%rbp\n\t"
        "mov $0, %%r8\n\t"
        "mov $0, %%r9\n\t"
        "mov $0, %%r10\n\t"
        "mov $0, %%r11\n\t"
        "mov $0, %%r12\n\t"
        "mov $0, %%r13\n\t"
        "mov $0, %%r14\n\t"
        "mov $0, %%r15\n\t"
        "ret\n\t"
        :: [sp]"r"(sp - 1)
    );
}

// inline int _start_c(size_t * sp) {
//     int argc = *sp;
//     char ** argv = (char **)(sp + 1);
//     char ** envp = &argv[argc + 1];

//     return main(argc, argv, envp);
// }

// __attribute__((naked, noreturn))
// void _start() {
//     asm(
//         "endbr64\n\t"
//         "xor %%rbp, %%rbp\n\t"
//         "mov %%rsp, %%rdi\n\t"
//         "andq $-16, %%rsp\n\t"
//         "call _start_c\n\t"
//         "movl %%eax, %%edi\n\t"
//         "movl $60, %%eax\n\t"
//         "syscall\n\t"
//         :::
//     );
// }
