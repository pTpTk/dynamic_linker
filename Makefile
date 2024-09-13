all:
	gcc -Wl,-soname,libloader.so -shared -static -nodefaultlibs -nostartfiles -o libloader.so -Wl,--dynamic-list=dynamic-list.txt crt.c main.c

test:
	gcc -Wl,-dynamic-linker,libloader.so -Wl,-z,lazy -nostartfiles -nodefaultlibs -o test test.o libputchar.so