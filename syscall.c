typedef unsigned long size_t;

size_t __syscall(size_t nr, size_t a0, size_t a1, size_t a2, size_t a3, size_t a4, size_t a5) {
    size_t ret;
    asm volatile(
        "mov %[a3], %%r10\n\t"
        "mov %[a4], %%r8\n\t"
        "mov %[a5], %%r9\n\t"
        "syscall\n\t"
        : "=a"(ret)
        : "a"(nr), "D"(a0), "S"(a1), "d"(a2), [a3]"rm"(a3), [a4]"rm"(a4), [a5]"rm"(a5)
        : "rcx", "r8", "r9", "r10"
    );
    return ret;
}

void * mmap(void *addr, size_t len, int prot, int flags, int fd, unsigned int offset) {
    return (void *) __syscall(9, (size_t)addr, len, prot, flags, fd, offset);
}

int open2(const char * file, int oflag, int mode) {
    __syscall(2, (size_t)file, oflag | 0100000, mode, 0, 0, 0);
}

int write2(int fd, const void * buf, size_t nbytes) {
    __syscall(1, fd, (size_t)buf, nbytes, 0, 0, 0);
}

int close2(int fd) {
    __syscall(3, fd, 0, 0, 0, 0, 0);
}