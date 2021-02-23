#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/io.h>

#define CNAME "myled"
#define RED 0
#define GREEN 1
#define BLUE 2
#define ON 1
#define OFF 0
#define RED_PHY_BASE 0xc001a000
#define BLUE_PHY_BASE 0xc001b000
#define GREEN_PHY_BASE 0xc001e000

unsigned int PHY_ADDR[] = {RED_PHY_BASE,GREEN_PHY_BASE,BLUE_PHY_BASE};
unsigned int * virt_addr[3];

int major=0;
char kbuf[128] = {0};

int myled_open(struct inode *inode, struct file *filp)
{
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	return 0;
}

ssize_t myled_read(struct file * filp, 
		char __user *ubuf, size_t size, loff_t *offs)
{
	int ret;

	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	if(size > sizeof(kbuf)) size = sizeof(kbuf);
	ret = copy_to_user(ubuf,kbuf,size);
	if(ret){
		printk("copy data to user error\n");
		return -EINVAL; //返回一个错误码
	}
	memset(kbuf,0,sizeof(kbuf));
	return size;
}
//ubuf[2]
//ubuf[0]  0 红  1 绿 2 蓝
//ubuf[1]  0 OFF   1 ON

ssize_t myled_write(struct file *filp, 
		const char __user *ubuf, size_t size, loff_t *offs)
{
	int ret;

	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	if(size > sizeof(kbuf)) size = sizeof(kbuf);
	ret = copy_from_user(kbuf,ubuf,size);
	if(ret){
		printk("copy data from user error\n");
		return -EINVAL; //返回一个错误码
	}

	switch(kbuf[0]){
		case RED:
			kbuf[1]?(*(virt_addr[0]) |= (1<<28)):\
				(*(virt_addr[0]) &= ~(1<<28));
			break;
		case GREEN:
			kbuf[1]?(*(virt_addr[1]) |= (1<<13)):\
				(*(virt_addr[1]) &= ~(1<<13));
			break;
		case BLUE:
			kbuf[1]?(*(virt_addr[2]) |= (1<<12)):\
				(*(virt_addr[2]) &= ~(1<<12));
			break;
	}

	return size;
}

int myled_close(struct inode *inode, struct file *filp)
{
	
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	return 0;
}

const struct file_operations fops = {
	.open  = myled_open,
	.read  = myled_read,
	.write = myled_write,
	.release = myled_close,
};

//入口
static int __init myled_init(void)
{
	int i;
	//注册字符设备驱动
	major = register_chrdev(0,CNAME,&fops);
	if(major < 0){
		printk("register LED char device driver error\n");
		return major;
	}
	for(i=0;i<ARRAY_SIZE(PHY_ADDR);i++){
		virt_addr[i] = ioremap(PHY_ADDR[i],40);
		if(virt_addr[i] == NULL){
			printk("ioremap error\n");
			return -ENOMEM;
		}
	}

	//virt_addr[0]  红灯的虚拟的基地址
	*(virt_addr[0] + 9) &= ~(3<<24);
	*(virt_addr[0] + 1) |= (1<<28);
	*(virt_addr[0] + 0) &= ~(1<<28);

	//virt_addr[1]  绿灯的虚拟的基地址
	*(virt_addr[1] + 8) &= ~(3<<26);
	*(virt_addr[1] + 1) |= (1<<13);
	*(virt_addr[1] + 0) &= ~(1<<13);
	
	//virt_addr[2]  蓝灯的虚拟的基地址
	*(virt_addr[2] + 8) &= ~(3<<24);
	*(virt_addr[2] + 8) |= (1<<25);
	*(virt_addr[2] + 1) |= (1<<12);
	*(virt_addr[2] + 0) &= ~(1<<12);

	return 0;
}
//出口
static void __exit myled_exit(void)
{
	int i;
	//注销字符设备驱动
	unregister_chrdev(major,CNAME);

	for(i=0;i<ARRAY_SIZE(virt_addr);i++){
		iounmap(virt_addr[i]);
	}
}

module_init(myled_init);
module_exit(myled_exit);
//许可证
MODULE_LICENSE("GPL");
MODULE_AUTHOR("farsight daizs_jt@hqyj.com");
