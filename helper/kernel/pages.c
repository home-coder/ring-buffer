//#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>

static unsigned long p = 0;
static unsigned long pp = 0;

static int __init shao_init(void)
{
	//分配共享内存（一个页面）

	p = __get_free_pages(GFP_KERNEL, 0);

	SetPageReserved(virt_to_page(p));
	// 使用virt_to_phys计算物理地址，供用户态程序使用
	pp = (unsigned long)virt_to_phys((void *)p);
	printk("<1> pp = 0x%lx\n", pp);

	//在共享内存中写上一个字符串
	
	strcpy((char *)p, "hello\n");
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
