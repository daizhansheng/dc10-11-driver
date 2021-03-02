#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/gpio.h>

#define GPIONO(m,n) (m*32+n)
#define RED GPIONO(0,28)
#define GREEN GPIONO(4,13)
#define BLUE GPIONO(1,12)

unsigned int led[] = {RED,GREEN,BLUE};

struct timer_list mytimer;
void timer_func(unsigned long data)
{
	unsigned int status = gpio_get_value(BLUE);
	//让灯亮或者灭
	gpio_set_value(BLUE,!status);  
	
	//再次启动定时器
	mod_timer(&mytimer,jiffies+HZ); 
}

static int __init mytimer_init(void)
{
	int i,ret;

	for(i=0;i<ARRAY_SIZE(led);i++){
		gpio_free(led[i]);
		ret = gpio_request(led[i],NULL);
		if(ret){
			printk("gpio request error\n");
			return ret;
		}

		gpio_direction_output(led[i],0);

	}
	
	
	mytimer.expires = jiffies + HZ; //1s
	mytimer.function = timer_func;
	mytimer.data = 0;
	init_timer(&mytimer);

	add_timer(&mytimer);
	
	return 0;
}
static void __exit mytimer_exit(void)
{
	int i;
	del_timer(&mytimer);
	for(i=0;i<ARRAY_SIZE(led);i++){
		gpio_free(led[i]);
	}
}
module_init(mytimer_init);
module_exit(mytimer_exit);
MODULE_LICENSE("GPL");




