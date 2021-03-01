#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>

#define LEFT 0
#define RIGHT 1
#define GPIONO(m,n) (m*32+n)
#define GPIOB8  GPIONO(1,8)
#define GPIOB16 GPIONO(1,16)

unsigned int gpiono[] = {GPIOB8,GPIOB16};
char *int_name[] = {"gpiob8_int","gpiob16_int"};

irqreturn_t key_irq_handler(int irqno, void *dev)
{
	int which = (int)dev;
	switch(which){
		case LEFT:
			printk("left key down........\n");
			break;
		case RIGHT:
			printk("right key down########\n");
			break;
	}
	
	return IRQ_HANDLED; //中断处理完成了
}

static int __init farsight_irq_init(void)
{
	int i,ret;
	//注册中断
	for(i=0;i<ARRAY_SIZE(gpiono);i++){
		ret = request_irq(gpio_to_irq(gpiono[i]),
			key_irq_handler,IRQF_TRIGGER_FALLING,
			int_name[i],(void *)i);
		if(ret){
			printk("register irq %d error\n",gpio_to_irq(gpiono[i]));
			return ret;
		}

	}
	return 0;
}

static void __exit farsight_irq_exit(void)
{
	int i;
	//注销中断
	for(i=0;i<ARRAY_SIZE(gpiono);i++){
		free_irq(gpio_to_irq(gpiono[i]),(void *)i);
	}

}
module_init(farsight_irq_init);
module_exit(farsight_irq_exit);
MODULE_LICENSE("GPL");




