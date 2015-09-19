
.PHONY : all libb clean

all : libb

libb : libb.c
	gcc -fPIC -c libb.c
	gcc -shared libb.o -o libb.so

clean :
	rm *.o libb.so

