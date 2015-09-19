
.PHONY : all clean

all :
	cd ./libc && make && cd ../
	cd ./libb && make && cd ../
	cd ./liba && make && cd ../
	cd ./dlopen && make && cd ../

clean :
	cd ./dlopen && make clean && cd ../
	cd ./liba && make clean && cd ../
	cd ./libb && make clean && cd ../
	cd ./libb && make && cd ../
