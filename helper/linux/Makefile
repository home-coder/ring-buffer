export ARCH=arm
CC = ~/basic/cross_compile/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/fsl-linaro-toolchain/bin/arm-fsl-linux-gnueabi-gcc
#CC =  gcc   
CFLAGS = -O2  -Wall --static 
INCLUDE = -I ./include 

all:mmap hello

mmap:mmap.o
	${CC} ${CFLAGS} mmap.o -o $@ 
	rm -rf *.o
hello:hello.o
	${CC} ${CFLAGS} hello.o -o $@ 

clean:  
	rm -rf *.o mmap 
