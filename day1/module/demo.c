#include <linux/init.h>
#include <linux/module.h>

//入口
static int __init demo_init(void)
{
	return 0;
}
//出口
static void __exit demo_exit(void)
{

}
//告诉内核入口，出口的地址
module_init(demo_init);
module_exit(demo_exit);
//许可证
MODULE_LICENSE("GPL");

