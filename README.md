# ring-buffer
1.最终参考下面通过时间 构造测试数据，首先测试kfifo性能
http://www.cnblogs.com/Anker/p/3481373.html

2.结合mmap函数，将mmap出来的数据当做测试数据，来测试kfifo的性能
注意mmap对应块设备如磁盘 USB等，但是不能对应流设备如串口等
另外参数length的长度必须是4096 的整数倍
