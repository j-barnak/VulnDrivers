KDIR = /lib/modules/`uname -r`/build

obj-m := uaf_driver.o
uaf_driver-objs := driver.o uaf.o

CFLAGS_driver.o := -DDEBUG
CFLAGS_uaf.o := -DDEBUG

.PHONY: build clean load unload

build: 
	make -C $(KDIR) M=`pwd`

clean:
	make -C $(KDIR) M=`pwd` clean

load:
	sudo insmod uaf_driver.ko
unload:
	sudo rmmod uaf_driver
