#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

int open2(const char * file, int oflag, int mode) {
    __syscall(2, (size_t)file, oflag | 0100000, mode, 0, 0, 0);
}

int write2(int fd, const void * buf, size_t nbytes) {
    __syscall(1, fd, (size_t)buf, nbytes, 0, 0, 0);
}

int close2(int fd) {
    __syscall(3, fd, 0, 0, 0, 0, 0);
}

int main() 
{ 
int sz; 
 
int fd = open2("foo.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644); 
if (fd < 0) 
{ 
    perror("r1"); 
    exit(1); 
} 
 
sz = write2(fd, "hello geeks\n", 12); 
 
printf("called write(% d, \"hello geeks\\n, %d)."
    " It returned %d, stdout = %d\n", fd, 12, sz, fileno(stdout)); 
 
close2(fd); 
}

// gcc systest.c -o test