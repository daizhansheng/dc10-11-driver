#include <linux/init.h>
#include <linux/module.h>
int backlight = 127;

module_param(backlight, int, 0775);
MODULE_PARM_DESC(backlight,"this is backlight val range(0-255)default:127");

unsigned char tt = 100;
module_param(tt,byte,0664);
MODULE_PARM_DESC(tt, "this is a char var");

char *p = "hello world";
module_param(p,charp,0775);
MODULE_PARM_DESC(p,"this is a charp var");

int num;
unsigned int ww[20] = {0};
module_param_array(ww,uint,&num, 0775);
MODULE_PARM_DESC(ww,"this is uint array [20]");
//入口,安装的时候执行
static int __init demo_init(void)
{
	int i;
	printk("backlight = %d\n",backlight);
	printk("tt = %d\n",tt);
	printk("p = %s\n",p);
	for(i=0;i<num;i++)
		printk("ww[%d] = %d\n",i,ww[i]);

	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	return 0;
}
//出口，卸载的时候执行
static void __exit demo_exit(void)
{
	printk("backlight = %d\n",backlight);
	printk("tt = %d\n",tt);
	printk("p = %s\n",p);
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
}
//告诉内核入口，出口的地址
module_init(demo_init);
module_exit(demo_exit);
//许可证
MODULE_LICENSE("GPL");

