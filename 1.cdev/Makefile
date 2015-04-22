# Если KERNELRELEASE определён, значит вызов сделан из
# системы сборки ядра и можно использовать её язык.
ifneq ($(KERNELRELEASE),)
obj-m := chdev.o
# Иначе вызов сделан прямо из командной
# строки; вызвать систему сборки ядра.
else
#KERNELDIR ?= /home/roma/1.Sources/5.Linux/linux-arm64
KERNELDIR ?= /lib/modules/$(shell uname -r)/build/
PWD := $(shell pwd)
default:	
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif

clean:
	rm -r *.ko mod* Mod* .c* *.o .ch* chdev.m* .tmp*
