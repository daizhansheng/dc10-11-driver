#include <linux/init.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/etherdevice.h>
#include <linux/types.h>

/*实验说明：
 * 其数据的接收与发送是针对用户空间而言的，
 *该实验与net2文件夹下的程序相比较添加了设置MAC地址以及发送数据包的计数；
 *实验步骤：
 *（1）ifconfig,  查看当前的网卡信息；
 *（2）ifconfig vent0 3.3.3.3 设置虚拟网卡的IP；
 *（3）ifconfig查看当前虚拟网卡的MAC地址以及收发包的数量及长度
 *（4）PING 3.3.3.5数据发送
 *（5）ctrl + c停止发送；ifconfig查看当前虚拟网卡的数据发送 及接收数据情况；
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
static int rx_packet(struct sk_buff *skb,struct net_device *dev)
{
	unsigned char * type;
	struct iphdr* ih;
	__be32 *saddr, *daddr, tmp;//源IP，目的IP，路由IP 
	unsigned char tmp_dev_addr[ETH_ALEN];
	struct ethhdr* ethhdr;

	struct sk_buff* rx_skb;

	/*从硬件读出/保存数据*/
	/*对调 mac头中的 源和目的,skb数据包格式：MAC（ 目的MAC+源MAC）+（其他信息）IP头（目的IP+源IP）+TYPE+数据*/
	ethhdr = (struct ethhdr*)skb->data;//skb-》data刚开始时MAC地址信息
	memcpy(tmp_dev_addr, ethhdr->h_dest, ETH_ALEN);//因为此时需要将接收到的数据包发送回去，故需要将目的MAC与源MAC对调位置；
	memcpy(ethhdr->h_dest,ethhdr->h_source , ETH_ALEN);
	memcpy(ethhdr->h_source, tmp_dev_addr, ETH_ALEN);

	/*对调 ip头中的源、目的*/
	ih = (struct iphdr*)(skb->data + sizeof(struct ethhdr));//SKB-》DATA + STRUCT ETHHDR 此时偏移到IP地址处
	saddr = &ih->saddr;
	daddr = &ih->daddr;                                                                                                                                                                                 

	tmp = *saddr;
	*saddr = *daddr;
	*daddr = tmp;

	type = skb->data + sizeof(struct ethhdr) + sizeof(struct iphdr);//SKB-》DATA   + SKB-》STRUCT ETHHDR + STRUCT IPHDR此时偏移到type
	*type = 0;  //0x80表示ping， 0表示reply

	ih->check = 0;//计算校验码，ip需要它。
	ih->check = ip_fast_csum((unsigned char*)ih, ih->ihl);
	//此处参考LDD3   17.6
	//构造一个sk_buff
	rx_skb  = dev_alloc_skb(skb->len + 2);
	skb_reserve(rx_skb,2);//对齐ip的16B边界
	memcpy(skb_put(rx_skb, skb->len),skb->data, skb->len );

	rx_skb->dev = dev;
	rx_skb->protocol = eth_type_trans(rx_skb, dev);
	rx_skb->ip_summed = CHECKSUM_UNNECESSARY;
	//更新统计信息
	ndev->stats.rx_packets++;
	ndev->stats.rx_bytes += skb->len;

	//提交sk_buff
	netif_rx(rx_skb);
	
	//释放sk_buff
	return 0;
}
//进行相关数据的发送
netdev_tx_t	net_start_xmit(struct sk_buff *skb,struct net_device *dev)
{
	/***********************************/
	//发送包，对于真实网卡会把数据包通过网卡发送
	netif_stop_queue(dev);//停止网卡发送队列
	rx_packet(skb,dev);//构造回复包，构造一个假的返包函数
	dev_kfree_skb(skb);//释放SKB
    netif_wake_queue(dev);//处理完队列的所有请求后唤醒队列
	/***********************************/
	count++;//发送的次数
	ndev->stats.tx_packets = count;//计算发送包的数量
	ndev->stats.tx_bytes += skb->len;//计算发送的字节数
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
	//申请网络设备结构体
	ndev = alloc_netdev(0,DEV_NAME,ether_setup);
	if(ndev == NULL){
		printk("ndev alloc fail!\n");
		return -ENOMEM;
	}
	ndev->netdev_ops = &net_ops;// 填充方法集合
   //设置下面两项才能通过ping
    ndev->flags |= IFF_NOARP;
    ndev->features |= NETIF_F_V6_CSUM;
	//注册网络设备
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







