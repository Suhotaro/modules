src := $(wildcard *.c *.h)
obj := $(patsubst %.c,%.o, $(filter %.c ,$(src)))
header := $(filter %.h ,$(src))
srt := $(sort $(obj) $(header))
#all := $(src) $(header)

.PHONY : all

all: util util2

util : util.c
	gcc $^ -g -o $@

util2 : util2.c
	gcc $^ -g -o $@

.PHONY : clean

clean:
	rm util util2
