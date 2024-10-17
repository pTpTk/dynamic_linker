#include "typedef.h"
#include "elf.h"
#include "syscall.c"
#include "util.c"

int main(int argc, char ** argv, char ** envp) {
    size_t * sp = (size_t *)(argv - 1);
    size_t * auxvals = (size_t *)envp;
    while (*auxvals++);

    void * base = NULL;
    void * entry = NULL;
    PHDR * phdr = NULL;
    for (size_t i = 0; auxvals[i] != AT_NULL; i += 2) {
        size_t val = auxvals[i + 1];
        switch (auxvals[i]) {
            case AT_ENTRY:
                entry = (void *)val;
                write2(1, "entry: ", 7);
                printAddr(entry);
                break;
            case AT_PHDR:
                phdr = (PHDR *)val;
                base = (void *)(val & 0xFFFFFFFFFFFFFF00);
                write2(1, "phdr: ", 6);
                printAddr(phdr);
                write2(1, "base: ", 6);
                printAddr(base);
                break;
        }
    }

    // handling program headers
    PHDR * ph_ptr = phdr;
    int ph_size = phdr->p_memsz;
    int ph_count = ph_size / 0x38;
    void * dynamic_section_addr = NULL;
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
                char* a = "DYNAMIC\n";
                write2(1, a, 8);
                dynamic_section_addr = (void *)ph_ptr->p_offset;
        }
    }

    dynamic_section_addr += (unsigned long long)base;
    write2(1, "dynamic section addr: ", 22);
    printAddr(dynamic_section_addr);
    
    Dynamic * dyn = (Dynamic *)dynamic_section_addr;
    void * strtab = NULL;
    void * needed_name = NULL;

    for(int i = 0; dyn[i].d_tag != DT_NULL; ++i) {
        switch(dyn[i].d_tag) {
            case DT_STRTAB:
            {
                strtab = (void *)(dyn[i].d_un.d_ptr);
                write2(1, "strtab: ", 8);
                printAddr(strtab);
                write2(1, "\n", 1);
                break;
            }
            case DT_NEEDED:
            {
                needed_name = (void *)(dyn[i].d_un.d_ptr);
                write2(1, "needed: ", 8);
                printAddr(needed_name);
                write2(1, "\n", 1);
                break;
            }
        }
    }

    write2(1, "needed: ", 8);
    needed_name += (size_t)base + (size_t)strtab;
    printAddr(needed_name);
    write2(1, "\n", 1);
    write2(1, needed_name, 20);
    write2(1, "\n", 1);

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
