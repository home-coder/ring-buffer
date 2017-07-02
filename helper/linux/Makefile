ARCH = x86
#ARCH = arm
ifeq ($(ARCH), arm)
    export ARCH=arm
    CC = ~/basic/cross_compile/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/fsl-linaro-toolchain/bin/arm-fsl-linux-gnueabi-gcc
else
	CC = gcc
endif

CFLAGS = -O2  --static -DDBG_TEST
INCLUDE = -I ./include 

all:mmap hello product-consumer

mmap:mmap.o
	${CC} ${CFLAGS} mmap.o -o $@ 
	rm -rf *.o
hello:hello.o
	${CC} ${CFLAGS} hello.o -o $@ 
product-consumer:product-consumer.o
	${CC} ${CFLAGS} product-consumer.o -o $@ -lpthread
clean:  
	rm -rf *.o mmap hello product-consumer 
