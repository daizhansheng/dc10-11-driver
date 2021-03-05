all:
	make -C /home/wangj/tftpboot/system/linux-3.14-OK.bak/ M=$(shell pwd) modules
	cp *.ko  /home/wangj/rootfs/
clean:
	make -C /home/wangj/tftpboot/system/linux-3.14-OK.bak/ M=$(shell pwd) clean 
obj-m = virt_net4.o

