#include <kfifo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct kfifo *kfifo_init(unsigned char *buffer, unsigned int size)
{
	struct kfifo *fifo;

	fifo = (struct kfifo *)malloc(sizeof(struct kfifo));
	if (!fifo)
		return NULL;

	fifo->buffer = buffer;
	fifo->size = size;
	fifo->in = fifo->out = 0;

	return fifo;
}

struct kfifo *kfifo_alloc(unsigned int size)
{
	unsigned char *buffer;
	struct kfifo *ret;
	if (size & (size - 1)) {
		fprintf(stderr, "size > 0x80000000n");
		size = roundup_pow_of_two(size);
	}

	buffer = (unsigned char *)malloc(size);
	if (!buffer)
		return NULL;

	ret = kfifo_init(buffer, size);

	if ((unsigned long)ret <= 0) {
		free(buffer);
	}

	return ret;
}

void kfifo_free(struct kfifo *fifo)
{
	free(fifo->buffer);
	free(fifo);
}

unsigned int __kfifo_put(struct kfifo *fifo, unsigned char *buffer,
			 unsigned int len)
{
	unsigned int l;

	len = min(len, fifo->size - fifo->in + fifo->out);

	l = min(len, fifo->size - (fifo->in & (fifo->size - 1)));
	memcpy(fifo->buffer + (fifo->in & (fifo->size - 1)), buffer, l);
	memcpy(fifo->buffer, buffer + l, len - l);

	fifo->in += len;
	return len;
}

unsigned int __kfifo_get(struct kfifo *fifo,
			 unsigned char *buffer, unsigned int len)
{
	unsigned int l;

	len = min(len, fifo->in - fifo->out);

	l = min(len, fifo->size - (fifo->out & (fifo->size - 1)));
	memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), l);

	fifo->out += len;
	return len;
}
