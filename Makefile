	
all:
	cd 1.cdev && make
	cd 2.util && make

clean:
	cd 2.util && make clean
	cd 1.cdev && make clean

