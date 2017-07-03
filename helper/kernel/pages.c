#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/delay.h>
#include <linux/sched.h>//wake_up_process()
#include <linux/kthread.h>//kthread_ceate(), kthread_run()

static unsigned long p = 0;
static unsigned long pp = 0;
int count = 0;
char buf[32];
static struct task_struct *mmap_task = NULL;

/*循环的写数据*/
static int put_pages(void *param)
{
	while (!kthread_should_stop()) { //不要用while 1, 因为那样rmmod会出错
		msleep(1);
		memset(buf, 0, 32);
		sprintf(buf, "hello %d", count++ % 1000);
		if ((void *)p == NULL) {
			printk("kerr, ---->p is null\n");
			return 0;
		}

		printk("put mmap -->%d\n", count);
		strcpy((char *)p, buf);
	}

	return 0;
}

static int creat_work(void)
{
	int err = -1;

	mmap_task = kthread_run(put_pages, NULL, "mmap_task");
	if (IS_ERR(mmap_task)) {
		printk("create new kernel thread failed\n");
		err = PTR_ERR(mmap_task);
		mmap_task = NULL;
		return err;
	}

	return 0;
}

static int __init kfifo_mmap_init(void)
{
	//分配共享内存（一个页面）
	p = __get_free_pages(GFP_KERNEL, 0);

	SetPageReserved(virt_to_page(p));
	// 使用virt_to_phys计算物理地址，供用户态程序使用
	pp = (unsigned long)virt_to_phys((void *)p);
	printk("<1> pp = 0x%lx\n", pp);

	//在共享内存中写上一个字符串
//	kernel_thread(put_pages, (void *)p, 0);
	
	creat_work();

	return 0;
}

static void __exit kfifo_mmap_exit(void)
{
	printk("----module exit----\n");
	if (mmap_task) {
		kthread_stop(mmap_task);
		mmap_task = NULL;
	}

	ClearPageReserved(virt_to_page(p));
	free_pages(p, 0);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("home-coder");
MODULE_DESCRIPTION("mmap test");
module_init(kfifo_mmap_init);
module_exit(kfifo_mmap_exit);
