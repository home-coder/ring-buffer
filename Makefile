CFLAGS = -O2  -Wall   
INCLUDE = -I ./include 
CC =  gcc   

test:test.o  kfifo.o  
	${CC} ${CFLAGS} test.o  kfifo.o -o $@ ${INCLUDE} -pthread  
	rm -rf *.o

test.o:  
	$(CC) -c test.c ${INCLUDE} -pthread  
kfifo.o:  
	$(CC) -c kfifo.c ${INCLUDE}  
clean:  
	rm -rf *.o test  
