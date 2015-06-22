#自己动手自作一个操作系统
###说明
* 参考《Orange's一个操作系统的实现》、《30天自制操作系统》。

* 这个操作系统目前只能通过软盘引导，同时为了方便编程，使用fat12格式化的软盘（fat文件系统头部信息已经写入boot.asm中），使用mount -o loop,rw a.img /xxx可以挂在，并且将其他程序写入软盘；相对的，linux 加载0扇区bootsec后，接着用bootsec加载1-4扇区的setup程序，setup继续引导剩余的全部扇区到内存，但是这样写入组织好的多个文件到软盘操作起来不是很方便，所以这里借用fat12格式的文件系统。

* 为了方便使用c语言，系统内核和未来的应用程序均使用ELF格式的可执行文件，这样就可以在Linux中编写程序。

* 运行环境：bochs release 2.6.7
* bochs调试断点：1.0x7c00 boot.bin入口地址；2.0x90100 loader.bin入口地址；3.0x400 kernel.bin入口地址

###目前进度:

1. boot loader 完成
2. 微内核架构kernel，主要处理进程调度
3. 自定义系统中断函数sendrecv，用于进程间消息通信
4. 屏幕输出，完成用户级别输出（printf）
5. 键盘输入
6. 简单的文件系统
7. 内存管理，fork进程
8. tty设备归入文件系统，由文件系统接口统一管理

###近期计划：
1. 支持树形目录
2. 软盘驱动

###问题：

* 操作系统调试很困难，打log的方式有时候都不可取，因为打log会操作文件系统，产生系统中断，会影响现场
* 微内核架构，所有操作都是异步的方式，编码调试非常困难
* 目前的问题是打开stdin stdout时会死锁阻塞进程，但是当调用printk系统中断时，死锁又会解除导致问题无从查找
