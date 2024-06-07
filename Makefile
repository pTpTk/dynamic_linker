gcc -Wl,-soname,libloader.so -shared -nodefaultlibs -nostartfiles -o libloader.so -Wl,--dynamic-list=dynamic-list.txt crt.c main.c libc_nonshared.a

gcc -Wl,-dynamic-linker,libloader.so -nostartfiles -nodefaultlibs -o test test.s