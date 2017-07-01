#include <stdio.h>
#include <pthread.h>
#include <strings.h>
#include <string.h>
#include <kfifo.h>

#define FIFO_LENGTH 4096
#define ORG_TEST

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
		read_len = kfifo_get(p->fifo, buffer, 25);
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
	unsigned int write_len = 0;
	unsigned int counter = 0;
	unsigned char buffer[32];
	struct ll_param *p = (struct ll_param *)param;

	for (counter = 0; counter < 100; counter++) {
		bzero(buffer, 32);
		sprintf((char *)buffer, "This is %d message.n", counter);
		write_len = kfifo_put(p->fifo, buffer, 25);	//strlen((char *)buffer)  
		usleep(100);
	}
}
#else
void thread_writer(void *param)
{

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
