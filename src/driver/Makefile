obj-m += x0driver.o
x0driver-objs := main.o ioctl.o
ccflags-y := -std=gnu99 -Wframe-larger-than=4096 -Wno-declaration-after-statement

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean