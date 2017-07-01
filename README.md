# ring-buffer
1.最终参考下面通过时间 构造测试数据，首先测试kfifo性能
http://www.cnblogs.com/Anker/p/3481373.html

2.结合mmap函数，将mmap出来的数据当做测试数据，来测试kfifo的性能
注意mmap对应块设备如磁盘 USB等，但是不能对应流设备如串口等
另外参数length的长度必须是4096 的整数倍


目前已通过helper目录下测试代码，测试mmap的使用，并使用内存设备直接代替具体的物理设备如dongle等。

下面就可以通过把helper的代码集成到test.c中就可以完全实现kfifo与mmap的综合应用了


未解决问题：
1.为什么要用环形缓冲区kfifo
2.内核数据写入，与mmap后的用户空间取走这个buffer的数据如何同步
3.内核模块的地址如何在用户空间操作
