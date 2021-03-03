#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/platform_device.h>

#define ADCNAME "myadc"
struct resource *res;
unsigned int irqno;
int major = 0;
struct class *cls = NULL;
struct device *dev = NULL;
unsigned int *virt_adc_base = NULL;
wait_queue_head_t wq;
int condition = 0;

int myadc_open(struct inode *inode, struct file *file)
{
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	return 0;
}
ssize_t myadc_read(struct file *file,
	char __user *ubuf, size_t size, loff_t *offs)
{
	int ret;
	unsigned int data;
	//1.开启adc转化
	writel(readl(virt_adc_base)|(1<<0),virt_adc_base); 
	
	//2.判断用户打开的方式是否是阻塞
	if(file->f_flags & O_NONBLOCK){
		printk("please open this driver use block mode\n");
		return -EINVAL;
	}
	
	//3.如果数据没有准备好就休眠
	ret = wait_event_interruptible(wq,condition);
	if(ret < 0){
		printk("wait error\n");
		return ret;
	}
	//4.被唤醒之后从硬件中读取数据
	data = readl(virt_adc_base+1);
	data = data & 0xfff;
	
	//5.将数据拷贝到用户空间
	if(size > sizeof(data)) size = sizeof(data);
	ret = copy_to_user(ubuf,(void *)&data,size);
	if(ret){
		printk("copy data to user error\n");
		return -EINVAL;
	}
	
	//6.将是否准备好数据的条件清零
	condition = 0;
	
	return size;
}
int myadc_close(struct inode *inode, struct file *file)
{
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	return 0;
}
const struct file_operations fops = {
	.open = myadc_open,
	.read = myadc_read,
	.release = myadc_close,
};

irqreturn_t adc_irq_handle(int irqno, void *dev)
{
	//1.唤醒休眠
	condition = 1;
	wake_up_interruptible(&wq);
	writel(readl(virt_adc_base+3)|(1<<0),virt_adc_base+3); //adc irq clear
	return IRQ_HANDLED;
}

int platform_adc_probe(struct platform_device *pdev)
{
	int ret;
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	//1.获取资源
	res = platform_get_resource(pdev,IORESOURCE_MEM,0);
	if(res == NULL){
			printk("platform get resource error\n");
			return -EAGAIN;
	}
	irqno = platform_get_irq(pdev,0);
	if(irqno < 0){
		printk("platform get irq error");
		return irqno;
	}
	
	printk("addr = %#x,irqno = %d\n",res->start,irqno);


	//2.注册字符设备驱动
	major = register_chrdev(0,ADCNAME,&fops);
	if(major < 0){
		printk("register char device driver error\n");
		return -EAGAIN;
	}


	//3.地址的映射
	virt_adc_base = ioremap(res->start,resource_size(res));
	if(virt_adc_base == NULL){
		printk("adc ioreamp error\n");
		return -ENOMEM;
	}
	
	//4.寄存器的初始化
	writel(0,virt_adc_base);
	writel(readl(virt_adc_base)|(6<<6),virt_adc_base); //ADCCON
	writel(0,virt_adc_base+4);
	writel(readl(virt_adc_base+4)|(149<<0),virt_adc_base+4); // PRESCALERCON
	writel(readl(virt_adc_base+4)|(1<<15),virt_adc_base+4); // PRESCALERCON 
	writel(readl(virt_adc_base+2)|(1<<0),virt_adc_base+2); //adc irq enable
	writel(readl(virt_adc_base+3)|(1<<0),virt_adc_base+3); //adc irq clear
	
	//5.中断的注册
	ret = request_irq(irqno,
	adc_irq_handle,IRQF_TRIGGER_NONE,ADCNAME,NULL);
	if(ret){
		printk("register adc irq error\n");
		return ret;
	}

	//6.等待队列头的初始化
	init_waitqueue_head(&wq);
	
	//7.自动创建设备节点
	cls = class_create(THIS_MODULE,ADCNAME);
	if(IS_ERR(cls)){
		printk("class create error\n");
		return PTR_ERR(cls);
	}

	dev = device_create(cls,NULL,MKDEV(major,0),NULL,ADCNAME);
	if(IS_ERR(dev)){
		printk("device create error\n");
		return PTR_ERR(dev);
	}

	return 0;
}
int plalform_adc_remove(struct platform_device *pdev)
{
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	device_destroy(cls,MKDEV(major,0));
	class_destroy(cls);
	free_irq(irqno,NULL);
	iounmap(virt_adc_base);
	unregister_chrdev(major,ADCNAME);
	return 0;
}

struct platform_driver platform_adc = {
	.probe = platform_adc_probe,
	.remove = plalform_adc_remove,
	.driver = {
		.name = "platform_adc"
	}

};

module_platform_driver(platform_adc);
MODULE_LICENSE("GPL");
