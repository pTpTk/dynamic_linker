all:
	gcc -Wl,-soname,libloader.so -shared -static -nodefaultlibs -nostartfiles -o libloader.so -Wl,--dynamic-list=dynamic-list.txt crt.c main.c rt_resolve.s

test:
	gcc -fPIC -c test.s -o test.o
	gcc -Wl,-dynamic-linker,libloader.so -Wl,-z,lazy -nostartfiles -nodefaultlibs -o test test.o libputchar.so

finder:
	gcc -fPIE -static-pie -nodefaultlibs -nostartfiles -o finder.o finder.c

putchar:
	gcc -fPIC -shared -nostartfiles putchar.s -o libputchar.so