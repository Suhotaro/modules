
.PHONY : all libc clean

all : libc

libc : libc.c
	gcc -fPIC -c libc.c
	gcc -shared libc.o  -o libc.so

clean :
	rm *.o libc.so

