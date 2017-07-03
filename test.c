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
pthread_cond_t q_not_full = PTHREAD_COND_INITIALIZER; 
pthread_cond_t q_not_empty = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

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
	unsigned int klen = 0;

	printf("nnn = %d\n", (int)roundup_pow_of_two(5));

	for (;;) {
		pthread_mutex_lock(&qlock);
		klen = kfifo_len(p->fifo);
		if (klen <= 0) {
			pthread_cond_wait(&q_not_empty, &qlock);
		}

		bzero(buffer, FIFO_LENGTH);
		read_len = kfifo_get(p->fifo, buffer, 32);
		if (read_len != 0) {
			printf("Read len:%d, buffer is  :< %s >n\n", read_len,
			       buffer);
		} else {
			counter++;
			if (counter > 2) {
				printf("这里会走到吗\n");
				break;
			}
		}

		pthread_mutex_unlock(&qlock);
		pthread_cond_signal(&q_not_full);
		//sleep(1);
		//usleep(100000);
	}
}

#ifdef ORG_TEST
void thread_writer(void *param)
{
	unsigned int counter = 0;
	unsigned char buffer[32];
	struct ll_param *p = (struct ll_param *)param;
	unsigned int klen = 0;

	for (counter = 0; counter < 2000; counter++) {
		pthread_mutex_lock(&qlock);

		klen = kfifo_len(p->fifo);
		if (klen >= FIFO_LENGTH) {
			sleep(5);
			pthread_cond_wait(&q_not_full, &qlock);
		}

		bzero(buffer, 32);
		sprintf((char *)buffer, "This is %d message.n", counter);
		kfifo_put(p->fifo, buffer, 32);	//strlen((char *)buffer)

		pthread_mutex_unlock(&qlock);
		pthread_cond_signal(&q_not_empty);
//		usleep(100);
	}
}

#else
void thread_writer(void *param)
{
#define PAGE_SIZE (4*1024)
//#define PAGE_OFFSET               0xc0000000 //32位的偏移3G, 但是我在内核求得用户空间地址了，这个变量不需要了
#define KERNEL_VIRT_ADDR 0x22049000	//此处地址即为内核模块打印的地址p，动态的不固定，需要自行修改
	unsigned char *buffer;
	int fd;
	unsigned long phy_addr;
	struct ll_param *p = (struct ll_param *)param;
	unsigned int klen = 0;

	fd = open("/dev/mem", O_RDWR);
	if (fd == -1)
		perror("open");

	phy_addr = KERNEL_VIRT_ADDR;
	buffer = mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, phy_addr);
	if (buffer == MAP_FAILED)
		perror("mmap");
	while (1) {
		pthread_mutex_lock(&qlock);

		if (buffer[0] != '\0') {
			if (klen >= FIFO_LENGTH) {
				pthread_cond_wait(&q_not_full, &qlock);
			}
			kfifo_put(p->fifo, buffer, 32);	//strlen((char *)buffer)  
			bzero(buffer, 32);
		}

		pthread_mutex_unlock(&qlock);
		pthread_cond_signal(&q_not_empty);
		usleep(10);
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
