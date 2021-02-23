#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#define CNAME "myled"
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
	//注册字符设备驱动
	major = register_chrdev(0,CNAME,&fops);
	if(major < 0){
		printk("register LED char device driver error\n");
		return major;
	}

	return 0;
}
//出口
static void __exit myled_exit(void)
{
	//注销字符设备驱动
	unregister_chrdev(major,CNAME);
}

module_init(myled_init);
module_exit(myled_exit);
//许可证
MODULE_LICENSE("GPL");
MODULE_AUTHOR("farsight daizs_jt@hqyj.com");
