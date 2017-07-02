#ARCH=arm
#与硬件相关，需要联调
ARCH=x86
#$(info, $(ARCH))//内核 androidmk使用
ifeq ($(ARCH), arm)
    CC = ~/basic/cross_compile/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/fsl-linaro-toolchain/bin/arm-fsl-linux-gnueabi-gcc
    CFLAGS = -O2  -Wall --static
else
    CC = gcc
    CFLAGS = -DORG_TEST -O2  -Wall --static
endif

INCLUDE = -I ./include 

test:test.o  kfifo.o  
	${CC} test.o  kfifo.o -o $@ ${INCLUDE} -lpthread $(CFLAGS)
	rm -rf *.o
test.o:  
	$(CC) -c test.c ${INCLUDE} -pthread $(CFLAGS)
kfifo.o:  
	$(CC) -c kfifo.c ${INCLUDE}  
clean:  
	rm -rf *.o test  
