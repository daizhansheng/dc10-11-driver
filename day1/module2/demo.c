#include <linux/init.h>
#include <linux/module.h>
int backlight = 127;

module_param(backlight, int, 0775);
MODULE_PARM_DESC(backlight,"this is backlight val range(0-255)default:127");
//入口,安装的时候执行
static int __init demo_init(void)
{
	printk("backlight = %d\n",backlight);
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	return 0;
}
//出口，卸载的时候执行
static void __exit demo_exit(void)
{
	printk("backlight = %d\n",backlight);
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
}
//告诉内核入口，出口的地址
module_init(demo_init);
module_exit(demo_exit);
//许可证
MODULE_LICENSE("GPL");

