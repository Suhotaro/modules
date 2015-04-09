# Если KERNELRELEASE определён, значит вызов сделан из
# системы сборки ядра и можно использовать её язык.
ifneq ($(KERNELRELEASE),)
obj-m := hello.o
# Иначе вызов сделан прямо из командной
# строки; вызвать систему сборки ядра.
else
KERNELDIR ?= /home/roma/1.Sources/5.Linux/linux-arm64
#KERNELDIR ?= /lib/modules/3.16.0-33-generic/build/

PWD := $(shell pwd)
default:
	cd /home/roma/1.Sources/5.Linux/linux-arm64
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif
