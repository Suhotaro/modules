src := $(wildcard *.c *.h)
obj := $(patsubst %.c, %.o, $(filter %.c, $(src)))

.PHONY : all dlopen clean

all : dlopen

dlopen : $(obj)
	gcc $^  -L../liba -la -ldl -o $@

clean :
	rm *.o
