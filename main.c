#include "typedef.h"
#include "elf.h"
#include "syscall.c"
#include "util.h"

extern void _runtime_resolve();

uint64_t runtime_resolve(Resolve * res, int64_t index) {
    uint64_t sym_index = res->r_jmprel[index].r_info;
    sym_index >>= 32;

    uint32_t st_name = res->r_symtab[sym_index].st_name;
    void * name = (void *)(res->r_strtab + st_name);

    PHDR * lib_phdr = (PHDR *)(res->r_lib + 0x40);
    int ph_size = lib_phdr->p_memsz;
    int ph_count = ph_size / 0x38;
    uint64_t dyn_addr = 0;
    for(int i = 1; i < ph_count; ++i, ++lib_phdr) {
        if(lib_phdr->p_type == 0x02) {
            dyn_addr = lib_phdr->p_offset;
            break;
        }
    }

    dyn_addr += (uint64_t)(res->r_lib);

    Dynamic * dyn = (Dynamic *)dyn_addr;
    
    void * strtab = NULL;
    Sym  * symtab = NULL;

    for(int i = 0; dyn[i].d_tag != DT_NULL; ++i) {
        switch(dyn[i].d_tag) {
            case DT_STRTAB:
            {
                strtab = (void *)(res->r_lib + dyn[i].d_un.d_ptr);
                break;
            }
            case DT_SYMTAB:
            {
                symtab = (Sym *)(res->r_lib + dyn[i].d_un.d_ptr);
                break;
            }
        }
    }

    uint64_t sym_count = (uint64_t)((uint64_t)strtab - (uint64_t)symtab) / 24;

    for(int i = 0; i < sym_count; ++i, ++symtab) {
        char * sym_name = (char *)(symtab->st_name + strtab);
        if(strcmp(name, sym_name) == 0) {
            break;
        }
    }
    
    uint64_t ret = (uint64_t)(res->r_lib) + (symtab->st_value);
    (res->r_pltgot)[index+3] = ret;
    return ret;

}

int main(int argc, char ** argv, char ** envp) {
    uint64_t * sp = (uint64_t *)(argv - 1);
    uint64_t * auxvals = (uint64_t *)envp;
    while (*auxvals++);

    void * base = NULL;
    void * entry = NULL;
    PHDR * phdr = NULL;
    for (uint64_t i = 0; auxvals[i] != AT_NULL; i += 2) {
        uint64_t val = auxvals[i + 1];
        switch (auxvals[i]) {
            case AT_ENTRY:
                entry = (void *)val;
                break;
            case AT_PHDR:
                phdr = (PHDR *)val;
                base = (void *)(val & 0xFFFFFFFFFFFFFF00);
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

        if(ph_ptr->p_type == 0x02) {
            dynamic_section_addr = (void *)ph_ptr->p_offset;
            break;
        }
    }

    dynamic_section_addr += (uint64_t)base;
    
    Dynamic * dyn = (Dynamic *)dynamic_section_addr;
    void * strtab = NULL;
    void * needed_name = NULL;
    void * pltgot = NULL;
    int64_t plt_rel_count = 0;
    void * jmprel = NULL;
    void * symtab = NULL;

    for(int i = 0; dyn[i].d_tag != DT_NULL; ++i) {
        switch(dyn[i].d_tag) {
            case DT_STRTAB:
            {
                strtab = (void *)(base + dyn[i].d_un.d_ptr);
                break;
            }
            case DT_NEEDED:
            {
                needed_name = (void *)(dyn[i].d_un.d_ptr);
                break;
            }
            case DT_PLTGOT:
            {
                pltgot = (uint64_t *)(base + dyn[i].d_un.d_ptr);
                break;
            }
            case DT_PLTRELSZ:
            {
                plt_rel_count = (int64_t)(dyn[i].d_un.d_val) / 24;
                break;
            }
            case DT_SYMTAB:
            {
                symtab = (void *)(base + dyn[i].d_un.d_ptr);
                break;
            }
            case DT_JMPREL:
            {
                jmprel = (void *)(base + dyn[i].d_un.d_ptr);
                break;
            }
        }
    }

    needed_name += (uint64_t)strtab;

    // library is in the same dir as the exe for simplicity
    int fd = open2(needed_name, 0, 0);
    uint64_t size = lseek2(fd, 0, 2);

    void * lib = mmap2(NULL, size, PROT_READ | PROT_EXEC, MAP_PRIVATE, fd, 0);
    close2(fd);

    Resolve * res = (Resolve *)mmap2(NULL, 32, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    res->r_jmprel = jmprel;
    res->r_symtab = symtab;
    res->r_strtab = strtab;
    res->r_lib = lib;
    res->r_pltgot = pltgot;

    uint64_t * pltgot_entry = pltgot;
    pltgot_entry[1] = (uint64_t)res;
    pltgot_entry[2] = (uint64_t)_runtime_resolve;

    // adjust temp got.plt entries
    for(int i = 0; i < plt_rel_count; ++i) {
        pltgot_entry[i+3] += (uint64_t)base;
    }

    sp[-1] = (uint64_t)entry;

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
