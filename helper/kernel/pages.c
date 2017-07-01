//#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>

static unsigned long p = 0;

static int __init shao_init(void)
{

	//分配共享内存（一个页面）

	p = __get_free_pages(GFP_KERNEL, 0);

	SetPageReserved(virt_to_page(p));

	printk("<1> p = 0x%08x\n", (unsigned int)p);

	//在共享内存中写上一个字符串

	strcpy((char *)p, "Hello world!\n");

	return 0;

}

static void __exit shao_exit(void)
{

	ClearPageReserved(virt_to_page(p));

	free_pages(p, 0);

}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kuanish");
MODULE_DESCRIPTION("mmap test");
module_init(shao_init);
module_exit(shao_exit);
