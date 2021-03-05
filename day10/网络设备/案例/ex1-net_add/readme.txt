使用说明：                                                                                                           
*1.insmod *.ko
*2.ifconfig----->查看原来的网络信息
*3.ifconfig vnet 3.3.3.3---->配置vnet的ip,(此时会自动调用open[dmesg])
*4.ifconfig---->查看配置后的网络信息
*5.ping 3.3.3.3---->ping自身，此时并没有进入驱动曾
*6.ping 3.3.3.4----->经过驱动曾,因为没有实现发送函数。故此时会导致出错


