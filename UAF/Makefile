KDIR  = /lib/modules/`uname -r`/build

kbuild: 
	make -C $(KDIR) M=`pwd`

clean:
	make -C $(KDIR) M=`pwd` clean

obj-m = uaf_driver.o
