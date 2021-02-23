#include <linux/init.h>
#include <linux/module.h>

//入口,安装的时候执行
static int __init demo_init(void)
{
	printk(KERN_WARNING "this is a warning message 1\n");
	printk(KERN_ERR "this is a warning message 2\n");
	printk("this is a warning message 3\n");
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	return 0;
}
//出口，卸载的时候执行
static void __exit demo_exit(void)
{
	printk(KERN_INFO "this is demo_exit 1\n");
	printk(KERN_ERR "this is demo_exit 2\n");
	printk("this is demo_exit 3\n");
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
}
//告诉内核入口，出口的地址
module_init(demo_init);
module_exit(demo_exit);
//许可证
MODULE_LICENSE("GPL");

