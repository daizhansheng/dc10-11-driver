#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/gpio.h>

struct timer_list mytimer;
struct work_struct work;

void  mywork(struct work_struct *data)
{
	//从mma8451q中读取数据
	//向上层上报数据
	printk("read data---->report data\n");
	
	mod_timer(&mytimer,jiffies+HZ);
}

void timer_func(unsigned long data)
{
	schedule_work(&work);
}

static int __init mytimer_init(void)
{
	INIT_WORK(&work,mywork);
	
	mytimer.expires = jiffies + HZ; //1s
	mytimer.function = timer_func;
	mytimer.data = 0;
	init_timer(&mytimer);

	add_timer(&mytimer);
	
	return 0;
}
static void __exit mytimer_exit(void)
{
	del_timer(&mytimer);

}
module_init(mytimer_init);
module_exit(mytimer_exit);
MODULE_LICENSE("GPL");




