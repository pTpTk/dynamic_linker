#include <stddef.h>

int main();
int _start_c(size_t * sp);

__attribute__((naked, noreturn))
void _start() {
    asm(
        "endbr64\n\t"
        "xor %%rbp, %%rbp\n\t"
        "mov %%rsp, %%rdi\n\t"
        "andq $-16, %%rsp\n\t"
        "call _start_c\n\t"
        "movl %%eax, %%edi\n\t"
        "movl $60, %%eax\n\t"
        "syscall\n\t"
        :::
    );
}

int _start_c(size_t * sp) {
    int argc = *sp;
    char ** argv = (char **)(sp + 1);
    char ** envp = &argv[argc + 1];

    int ret = main(argc, argv, envp);

    return ret;
}