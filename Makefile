export ARCH=arm
CC = ~/basic/cross_compile/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/fsl-linaro-toolchain/bin/arm-fsl-linux-gnueabi-gcc
#CC =  gcc   
CFLAGS = -O2  -Wall   
INCLUDE = -I ./include 

test:test.o  kfifo.o  
	${CC} ${CFLAGS} test.o  kfifo.o -o $@ ${INCLUDE} -pthread  
	rm -rf *.o

test.o:  
	$(CC) -c test.c ${INCLUDE} -pthread  
kfifo.o:  
	$(CC) -c kfifo.c ${INCLUDE}  
clean:  
	rm -rf *.o test  
