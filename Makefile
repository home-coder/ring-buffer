#ARCH=armh
#ARCH=armc
#与硬件相关，需要联调
#ARCH=x86org
#ARCH=x86mmap
ARCH=amlogic

#$(info, $(ARCH))//内核 androidmk使用
ifeq ($(ARCH), armh)
    CC = ~/basic/cross_compile/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/fsl-linaro-toolchain/bin/arm-fsl-linux-gnueabi-gcc
    CFLAGS = -O2  -Wall --static
endif

ifeq ($(ARCH), armc)
	CC = /opt/toolchain/mstar/linaro-aarch64_linux-2014.09_843419-patched/bin/aarch64-linux-gnu-gcc 
    CFLAGS = -O2  -Wall --static
endif

ifeq ($(ARCH), amlogic)
    CC = /opt/toolchain/amlogic/gcc-linaro-aarch64-linux-gnu-4.9-2014.09_linux/bin/aarch64-linux-gnu-gcc
    CFLAGS = -O2  -Wall --static
endif

ifeq ($(ARCH), x86mmap)
	CC = gcc
	CFLAGS = -O2 -Wall --static
endif

ifeq ($(ARCH), x86org)
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
