#include <stdio.h>
#include <pthread.h>
#include <strings.h>
#include <string.h>
#include <kfifo.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define FIFO_LENGTH 4096

struct ll_param {
	struct kfifo *fifo;
	int msg_len;
};

static struct ll_param fifo;

void thread_reader(void *param)
{
	int read_len = 0;
	unsigned int counter = 0;
	unsigned char buffer[FIFO_LENGTH];
	struct ll_param *p = (struct ll_param *)param;

	printf("nnn = %d\n", (int)roundup_pow_of_two(5));

	for (;;) {
		bzero(buffer, FIFO_LENGTH);
		read_len = kfifo_get(p->fifo, buffer, 32);
		if (read_len != 0) {
			printf("Read len:%d, buffer is  :< %s >n\n", read_len,
			       buffer);
		} else {
			counter++;
		}
		if (counter > 20) {
			break;
		}
		usleep(50000);
	}
}

#ifdef ORG_TEST
void thread_writer(void *param)
{
	unsigned int counter = 0;
	unsigned char buffer[32];
	struct ll_param *p = (struct ll_param *)param;

	for (counter = 0; counter < 100; counter++) {
		bzero(buffer, 32);
		sprintf((char *)buffer, "This is %d message.n", counter);
		kfifo_put(p->fifo, buffer, 32);	//strlen((char *)buffer)  
		usleep(100);
	}
}
#else
void thread_writer(void *param)
{
#define PAGE_SIZE (4*1024)
#define PAGE_OFFSET               0xc0000000 //32位的偏移3G, 但是我在内核求得用户空间地址了，这个变量不需要了
#define KERNEL_VIRT_ADDR 0x2249c000	//此处地址即为内核模块打印的地址p，动态的不固定，需要自行修改
	unsigned char *buffer;
	int fd;
	unsigned long phy_addr;
	struct ll_param *p = (struct ll_param *)param;

	fd = open("/dev/mem", O_RDWR);
	if (fd == -1)
		perror("open");

	phy_addr = KERNEL_VIRT_ADDR;
	buffer = mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, phy_addr);
	if (buffer == MAP_FAILED)
		perror("mmap");
	while (1) {
		kfifo_put(p->fifo, buffer, 32);	//strlen((char *)buffer)  
	}

	munmap(buffer, PAGE_SIZE);
	close(fd);
}
#endif

int main(void)
{
	pthread_t pidr;
	pthread_t pidw;

	fifo.msg_len = 10;
	fifo.fifo = kfifo_alloc(FIFO_LENGTH);

	pthread_create(&pidw, NULL, (void *)thread_writer, &fifo);
#ifdef ORG_TEST
	pthread_create(&pidr, NULL, (void *)thread_reader, &fifo);
#else
	pthread_create(&pidr, NULL, (void *)thread_reader, &fifo);
#endif
	pthread_join(pidr, NULL);
	pthread_join(pidw, NULL);

	kfifo_free(fifo.fifo);
	printf("nGoodbye!n\n");
	return 0;
}
