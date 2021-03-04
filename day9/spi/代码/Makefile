#KERNELDIR:= /lib/modules/$(shell uname -r)/build/
KERNELDIR := /home/linux/kernel/kernel-3.4.39/
PWD:=$(shell pwd)
all:
	make -C $(KERNELDIR) M=$(PWD) modules
	arm-none-linux-gnueabi-gcc wr_test.c
	cp *.ko a.out  ~/rootfs/
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
	rm a.out
obj-m:=spi_driver.o
