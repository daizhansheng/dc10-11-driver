#include <linux/init.h>
#include <linux/module.h>
#include <linux/netdevice.h>
/*实验说明：
 *该实验与net2文件夹下的程序相比较添加了设置MAC地址以及发送数据包的计数；
 *实验步骤：
 *（1）ifconfig,  查看当前的网卡信息；
 *（2）ifconfig vent0 3.3.3.3 设置虚拟网卡的IP；
 *（3）ifconfig查看当前虚拟网卡的MAC地址以及收发包的数量及长度
 *（4）PING 3.3.3.5数据发送
 *（5）ctrl + c停止发送；ifconfig查看当前虚拟网卡的数据发送情况；
 *（6) rmmod virt_net1, 卸载模块；此时查看内核打印信息，调用stop;
 */
#define DEV_NAME "vnet0"		//网络设备命名
static struct net_device *ndev = NULL; //网络设备结构体定义
static int count = 0;
//打开相应的虚拟网卡
int	net_open(struct net_device *dev)
{
	ndev->dev_addr[0] = 00;	/*设置网络的MAC地址*/
	ndev->dev_addr[1] = 01;
	ndev->dev_addr[2] = 02;
	ndev->dev_addr[3] = 03;
	ndev->dev_addr[4] = 04;
	ndev->dev_addr[5] = 05;
	printk("%s%d\n",__func__,__LINE__);
	return 0;
}
//关闭相应的虚拟网卡
int	net_stop(struct net_device *dev)
{
	printk("%s%d\n",__func__,__LINE__);
	return 0;
}
//进行相关数据的发送
netdev_tx_t	net_start_xmit(struct sk_buff *skb,struct net_device *dev)
{
	count++;
	ndev->stats.tx_packets = count;
	ndev->stats.tx_bytes += skb->len;
	printk("%s%d,count:%d\n",__func__,__LINE__,count);
	return NETDEV_TX_OK;
}
//网络设备操作方法集合
struct net_device_ops net_ops = {
	.ndo_open = net_open,
	.ndo_stop = net_stop,
	.ndo_start_xmit = net_start_xmit,
};

//模块初始化
static int __init netdev_init(void)
{
	ndev = alloc_netdev(0,DEV_NAME,ether_setup);
	if(ndev == NULL){
		printk("ndev alloc fail!\n");
		return -ENOMEM;
	}
	ndev->netdev_ops = &net_ops;
	register_netdev(ndev);
	printk("net_init ok !\n");
	return 0;
}
//模块退出，注意先注销设备之后再释放空间，否则卸载会出错
static void __exit netdev_exit(void)
{
	unregister_netdev(ndev);//注销网络设备
	free_netdev(ndev);		//释放ndev的空间
	printk("net_exit ok !\n");
}

module_init(netdev_init);
module_exit(netdev_exit);
MODULE_LICENSE("GPL");







