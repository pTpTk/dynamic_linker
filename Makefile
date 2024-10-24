all:
	gcc -Wl,-soname,libloader.so -shared -static -nodefaultlibs -nostartfiles -o libloader.so -Wl,--dynamic-list=dynamic-list.txt crt.c main.c -g

test:
	gcc -fPIC -c test.s -o test.o -g
	gcc -Wl,-dynamic-linker,libloader.so -Wl,-z,lazy -nostartfiles -nodefaultlibs -o test test.o libputchar.so -g

finder:
	gcc -fPIE -static-pie -nodefaultlibs -nostartfiles -o finder.o finder.c

putchar:
	gcc -fPIC -shared -nostartfiles putchar.s -o libputchar.so