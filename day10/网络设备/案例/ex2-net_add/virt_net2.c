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
 *6.ping 3.3.3.4----->经过驱动曾，调用发送函数
 *7.rmmod *----->删除模块，调用remove-->stop[dmesg]
 */
static int count;       //定义数据传输次数
//定义网络设备结构体
struct net_device *vnet = NULL;

//打开
int	net_open(struct net_device *dev)
{
	printk("%s%s%d\n",__FILE__,__func__,__LINE__);
	return 0;
}
//停止
int	net_stop(struct net_device *dev)
{
	printk("%s%s%d\n",__FILE__,__func__,__LINE__);
	return 0;
}
//发送
netdev_tx_t hard_start_xmit(struct sk_buff *skb,struct net_device *dev)
{
	printk("%s%s%d,count:%d\n",__FILE__,__func__,__LINE__,count++);
	return NETDEV_TX_OK;
}
//控制
int net_ioctl(struct net_device *dev,struct ifreq *ifr, int cmd)
{
	printk("%s%s%d\n",__FILE__,__func__,__LINE__);
	return 0;
}
//网络设备操作方法集合
struct net_device_ops netdev_ops = {
	.ndo_open = net_open,
	.ndo_stop = net_stop,
	.ndo_start_xmit = hard_start_xmit,
	.ndo_do_ioctl = net_ioctl,

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




