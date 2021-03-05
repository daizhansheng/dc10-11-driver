#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/netdevice.h>

#define NET_NAME "vnet"
/* 使用说明：
 *1.insmod *.ko
 *2.ifconfig----->查看原来的网络信息
 *3.ifconfig vnet 3.3.3.3---->配置vnet的ip,(此时会自动调用open[dmesg])
 *4.ifconfig---->查看配置后的网络信息
 *5.ping 3.3.3.3---->ping自身，此时并没有进入驱动曾
 *6.ping 3.3.3.4----->经过驱动曾,因为没有实现发送函数。故此时会导致出错
 */
//定义网络设备结构体
struct net_device *vnet = NULL;
//网络设备操作方法集合[必须写操作方法集合否则出错]
struct net_device_ops netdev_ops = {
};
//模块初始化
static int __init net_init(void)//网卡初始化
{
	printk("%s%s%d\n",__FILE__,__func__,__LINE__);
	//申请网络结构体
	vnet = alloc_netdev(0,NET_NAME,ether_setup);
	if(vnet==NULL){
		printk("alloc netdev fail!\n");
		return -ENOMEM;
	}
	vnet->netdev_ops = &netdev_ops;
	register_netdev(vnet);//注册网络设备
	return 0;
}
//模块退出
static void __exit net_exit(void)//网卡退出
{
	printk("%s%s%d\n",__FILE__,__func__,__LINE__);
	unregister_netdev(vnet);//注销网络设备
	free_netdev(vnet);// 释放网络设备
}

module_init(net_init);//注册
module_exit(net_exit);//注销
MODULE_LICENSE("GPL");//许可




