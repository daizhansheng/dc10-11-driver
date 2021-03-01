#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/poll.h>

#define CNAME "mycdev"
#define COUNT 3
#define RED 0
#define GREEN 1
#define BLUE 2

struct cdev *cdev; 
int major = 0;
int minor = 0;
struct class*cls;
struct device *dev;
char kbuf[128] = {0};
struct fasync_struct * fapp;

int mycdev_open(struct inode *inode, struct file *filp)
{
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	

	return 0;
}

ssize_t  mycdev_read(struct file * filp, 
		char __user *ubuf, size_t size, loff_t *offs)
{
	int ret;
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	if(size > sizeof(kbuf)) size = sizeof(kbuf);
	ret = copy_to_user(ubuf,kbuf,size);
	if(ret){
		printk("copy data to user error\n");
		return -EINVAL; 
	}

	return size;
}

ssize_t  mycdev_write(struct file *filp, 
		const char __user *ubuf, size_t size, loff_t *offs)
{
	int ret;
	
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	if(size > sizeof(kbuf)) size = sizeof(kbuf);
	ret = copy_from_user(kbuf,ubuf,size);
	if(ret){
		printk("copy data from user error\n");
		return -EINVAL; 
	}

	//发信号
	kill_fasync(&fapp,SIGIO,POLL_IN);
	
	return size;
}

int mycdev_fasync(int fd, struct file *filp, int on)
{
	return fasync_helper(fd,filp,on,&fapp);
}

int  mycdev_close(struct inode *inode, struct file *filp)
{
	
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	return 0;
}

const struct file_operations fops = {
	.open  =  mycdev_open,
	.read  =  mycdev_read,
	.write =  mycdev_write,
	.fasync = mycdev_fasync,
	.release =  mycdev_close,
};

static int __init mycdev_init(void)
{
	int ret,i;
	dev_t devno;
	//1.分配对象
	cdev = cdev_alloc();
	if(cdev == NULL){
		printk("alloc cdev memory error\n");
		ret = -ENOMEM;
		goto ERR1;
	}

	//2.对象的初始化
	cdev_init(cdev,&fops);
	
	//3.申请设备号
	if(major > 0){
		ret = register_chrdev_region(MKDEV(major,minor),
			COUNT,CNAME);
		if(ret){
			printk("static: alloc device number error\n");
			goto ERR2;
		}
	}else{
		ret = alloc_chrdev_region(&devno,minor,
			COUNT,CNAME);
		if(ret){
			printk("dynamic: alloc device number error\n");
			goto ERR2;
		}
		major = MAJOR(devno);
		minor = MINOR(devno);
	}

	//4.注册
	ret = cdev_add(cdev,MKDEV(major,minor),COUNT);
	if(ret){
		printk("register char device driver error\n");
		goto ERR3;
	}

	//5.自动创建设备节点
	cls = class_create(THIS_MODULE,CNAME);
	if(IS_ERR(cls)){
		printk("class create error\n");
		ret = PTR_ERR(cls);
		goto ERR4;
	}
	for(i=0;i<COUNT;i++){
		dev = device_create(cls,NULL,MKDEV(major,minor+i),
			NULL,"mycdev%d",i);
		if(IS_ERR(dev)){
			printk("device create error\n");
			ret = PTR_ERR(dev);
			goto ERR5;
		}
	}
	
	return 0; //这里的return千万不要忘记写!!!!
ERR5:
	for(--i;i>=0;i--){
		device_destroy(cls,MKDEV(major,minor+i));
	}
	class_destroy(cls);
ERR4:
	cdev_del(cdev);
ERR3:
	unregister_chrdev_region(MKDEV(major,minor),COUNT);
ERR2:
	kfree(cdev);
ERR1:
	return ret;
	
}

static void __exit mycdev_exit(void)
{
	int i;
	for(i=0;i<COUNT;i++){
		device_destroy(cls,MKDEV(major,minor+i));
	}
	class_destroy(cls);

	cdev_del(cdev);

	unregister_chrdev_region(MKDEV(major,minor),COUNT);

	kfree(cdev);	
}
module_init(mycdev_init);
module_exit(mycdev_exit);
MODULE_LICENSE("GPL");



