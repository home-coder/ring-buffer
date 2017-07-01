#ifndef _Linux_KFIFO_H
#define _Linux_KFIFO_H

#define __u32 unsigned long
#define __u64 unsigned long long

#define min(x,y) ((x) < (y) ? (x) : (y) )

#define max(x,y) ((x) > (y) ? (x) : (y) )
/* 
   static inline int fls(int x) 
   { 
   int r; 

   __asm__("bsrl %1,%0nt" 
   "jnz 1fnt" 
   "movl $-1,%0n" 
   "1:" : "=r" (r) : "rm" (x)); 
   return r+1; 
   } 

*/
static inline int fls(int x)
{
	int r = 32;

	if (!x)
		return 0;
	if (!(x & 0xffff0000u)) {
		x <<= 16;
		r -= 16;
	}
	if (!(x & 0xff000000u)) {
		x <<= 8;
		r -= 8;
	}
	if (!(x & 0xf0000000u)) {
		x <<= 4;
		r -= 4;
	}
	if (!(x & 0xc0000000u)) {
		x <<= 2;
		r -= 2;
	}
	if (!(x & 0x80000000u)) {
		x <<= 1;
		r -= 1;
	}
	return r;
}

static inline int fls64(__u64 x)
{
	__u32 h = x >> 32;
	if (h)
		return fls(h) + 32;
	return fls(x);
}

static inline unsigned fls_long(unsigned long l)
{
	if (sizeof(l) == 4)
		return fls(l);
	return fls64(l);
}

static inline unsigned long roundup_pow_of_two(unsigned long x)
{
	return 1UL << fls_long(x - 1);
}

struct kfifo {
	unsigned char *buffer;	/* the buffer holding the data */
	unsigned int size;	/* the size of the allocated buffer */
	unsigned int in;	/* data is added at offset (in % size) */
	unsigned int out;	/* data is extracted from off. (out % size) */
};

struct kfifo *kfifo_init(unsigned char *buffer, unsigned int size);
struct kfifo *kfifo_alloc(unsigned int size);
void kfifo_free(struct kfifo *fifo);
unsigned int __kfifo_put(struct kfifo *fifo, unsigned char *buffer,
			 unsigned int len);
unsigned int __kfifo_get(struct kfifo *fifo, unsigned char *buffer,
			 unsigned int len);

static inline void __kfifo_reset(struct kfifo *fifo)
{
	fifo->in = fifo->out = 0;
}

static inline void kfifo_reset(struct kfifo *fifo)
{

	__kfifo_reset(fifo);

}

static inline unsigned int kfifo_put(struct kfifo *fifo,
				     unsigned char *buffer, unsigned int len)
{
	unsigned int ret;

	ret = __kfifo_put(fifo, buffer, len);

	return ret;
}

static inline unsigned int kfifo_get(struct kfifo *fifo,
				     unsigned char *buffer, unsigned int len)
{
	unsigned int ret;

	ret = __kfifo_get(fifo, buffer, len);

	if (fifo->in == fifo->out)
		fifo->in = fifo->out = 0;

	return ret;
}

static inline unsigned int __kfifo_len(struct kfifo *fifo)
{
	return fifo->in - fifo->out;
}

static inline unsigned int kfifo_len(struct kfifo *fifo)
{
	unsigned int ret;

	ret = __kfifo_len(fifo);

	return ret;
}

#endif
