KERNELDIR:=/home/linux/kernel/kernel-3.4.39/
#KERNELDIR:=/lib/modules/$(shell uname -r)/build
PWD:= $(shell pwd)

all:
	make -C $(KERNELDIR) M=$(PWD) modules
clean:
	make -C $(KERNELDIR) M=$(PWD) clean

obj-m:=mytimer_work.o
