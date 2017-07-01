#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#define PAGE_SIZE (4*1024)
#define PAGE_OFFSET               0xc0000000
#define KERNEL_VIRT_ADDR 0x22cb7000	//此处地址即为内核模块打印的地址p，动态的不固定，需要自行修改

int main()
{
	char *buf;
	int fd;
	unsigned long phy_addr;

	fd = open("/dev/mem", O_RDWR);
	if (fd == -1)
		perror("open");

	phy_addr = KERNEL_VIRT_ADDR;
	buf = mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, phy_addr);
	if (buf == MAP_FAILED)
		perror("mmap");
	while (1)
	puts(buf);					//打印共享内存的内容
	munmap(buf, PAGE_SIZE);
	close(fd);

	return 0;

}
